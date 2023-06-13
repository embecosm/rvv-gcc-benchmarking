// Test program for scalar memset and memcpy performance.

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

extern void *smemcpy(void *__restrict dest, const void *__restrict src,
		     size_t n);
extern void *smemset(void *s, int c, size_t n);

// Benchmark function for scalar memcpy
static uint64_t
run_smemcpy (size_t len)
{
  uint64_t cstart = ccount ();
  uint64_t cend = ccount ();
  uint8_t *src = (uint8_t *) malloc (len);
  uint8_t *dst = (uint8_t *) malloc (len);

  mem_init_random (src, len);
  cstart = ccount ();
  smemcpy (dst, src, len);
  cend = ccount ();
  free (dst);
  free (src);
  return cdiff (cstart, cend);
}

int
main (int argc, char* argv[])
{
  uint64_t coverhead;		// Overhead of calling cycle count
  int sizes [1000];		// The sizes to try
  uint64_t res [1000];
  size_t num_sizes;		// Number of sizes

  // Work out cost of measuring time. Discard the first WARMUP to ensure caches
  // are hot.
  coverhead = run_bm (&run_overhead, 0, 0);
  printf ("Average cycle count measurement overhead %llu\n", coverhead);

  // Generate a vector of sizes to try
  num_sizes = genseq (sizes);

  // Try scalar memcpy of various sizes up to 1MB. Do multiple runs to get
  // caches filled.
  printf ("\nStandard memcpy (scalar compilation) cycles\n\n");
  printf ("    len    cycles   per byte\n");
  printf ("    ---    ------   --------\n");

  for (size_t i = 0; i < num_sizes; i++)
    {
      uint64_t cdelta = run_bm (&run_smemcpy, sizes[i], coverhead);
      float c_per_b = ((float) cdelta) / ((float) sizes[i]);

      res[i] = cdelta;
      printf ("%7d  %8llu  %9.2f\n", sizes[i], cdelta, c_per_b);
    }

  printf ("\n");

  for (size_t i = 0; i < num_sizes; i++)
      printf ("%d,%d\n", sizes[i], res[i]);

  printf ("\n");
}

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
