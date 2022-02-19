// SPDX-FileCopyrightText: Copyright (c) 2019-2021 Virginia Tech
// SPDX-License-Identifier: Apache-2.0

#include <numa-config.h>

#include "pactree.h"

#define NUMDATA 10'000'000

int main (int argc, char** argv) {
    remove ("/mnt/pmem0/log");  // delete the mapped file.
    remove ("/mnt/pmem0/sl");   // delete the mapped file.
    remove ("/mnt/pmem0/dl");   // delete the mapped file.
    remove ("/mnt/pmem1/log");  // delete the mapped file.
    remove ("/mnt/pmem1/sl");   // delete the mapped file.
    remove ("/mnt/pmem1/dl");   // delete the mapped file.

    pactree* pt = new pactree (1);
    pt->registerThread ();

    for (int i = 1; i < NUMDATA; i++) {
        pt->insert (i, i);
    }

    for (int i = 1; i < NUMDATA; i++) {
        if (i != pt->lookup (i)) {
            printf ("error");
            exit (1);
        }
    }
}
