// SPDX-FileCopyrightText: Copyright (c) 2019-2021 Virginia Tech
// SPDX-License-Identifier: Apache-2.0

#include "WorkerThread.h"

#include <assert.h>
#include <libpmemobj.h>

#include "Combiner.h"
#include "Oplog.h"
#include "logger.h"
#include "pactree.h"

WorkerThread::WorkerThread (int id, int activeNuma) {
    this->workerThreadId = id;
    this->activeNuma = activeNuma;
    this->workQueue = &g_workQueue[workerThreadId];
    this->logDoneCount = 0;
    this->opcount = 0;
    if (id == 0) freeQueue = new std::queue<std::pair<uint64_t, void*>>;
}

bool WorkerThread::applyOperation () {
    std::vector<OpStruct*>* oplog = workQueue->front ();
    int numaNode = workerThreadId % activeNuma;
    SearchLayer* sl = g_perNumaSlPtr[numaNode];
    uint8_t hash = static_cast<uint8_t> (workerThreadId / activeNuma);
    bool ret = false;
    int i = 0;
    for (auto opsPtr : *oplog) {
        OpStruct& ops = *opsPtr;
        if (ops.hash != hash) continue;
        opcount++;
        DEBUG ("Apply log 0x%lx. op (%3d): %u, key: %lu, poolId: %u, newKey: %lu, newVal: %lu ",
               oplog, i++, ops.op, ops.key, ops.poolId, ops.newKey, ops.newVal);
        if (ops.op == OpStruct::insert) {
            void* newNodePtr = reinterpret_cast<void*> (((unsigned long)(ops.poolId)) << 48 |
                                                        (ops.newNodeOid.off));
            sl->insert (ops.key, newNodePtr);
            opsPtr->op = OpStruct::done;
        } else if (ops.op == OpStruct::remove) {
            sl->remove (ops.key, ops.oldNodePtr);
            opsPtr->op = OpStruct::done;
            flushToNVM ((char*)&ops, sizeof (OpStruct));
            if (workerThreadId == 0) {
                std::pair<uint64_t, void*> removePair;
                removePair.first = logDoneCount;
                removePair.second = ops.oldNodePtr;
                freeQueue->push (removePair);
                ret = true;
            }
        } else {
            if (ops.op == OpStruct::done) {
                ERROR (
                    "Apply a done log 0x%lx. op: %u, key: %lu, poolId: %u, newKey: %lu, "
                    "newVal: %lu ",
                    oplog, ops.op, ops.key, ops.poolId, ops.newKey, ops.newVal);
                assert (false);
                perror ("Apply done oplog");
                exit (1);
            }
            perror ("What the heck?");
            exit (1);
        }
        flushToNVM ((char*)opsPtr, sizeof (OpStruct));
        smp_wmb ();
    }
    workQueue->pop ();
    logDoneCount++;
    return ret;
}

bool WorkerThread::isWorkQueueEmpty () { return !workQueue->read_available (); }

void WorkerThread::freeListNodes (uint64_t removeCount) {
    assert (workerThreadId == 0 && freeQueue != NULL);
    if (freeQueue->empty ()) return;
    while (!freeQueue->empty ()) {
        std::pair<uint64_t, void*> removePair = freeQueue->front ();
        if (removePair.first < removeCount) {
            PMEMoid ptr = pmemobj_oid (removePair.second);
            pmemobj_free (&ptr);
            freeQueue->pop ();
        } else
            break;
    }
}
