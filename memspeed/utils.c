// Utility functions

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// Compare two integers
static int
cmpint (const void *first, const void *second)
{
  int *a = (int *) first;
  int *b = (int *) second;
  return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

// Generate a sequence of memory sizes. Return a pointer to an array
size_t
genseq (int seq[])
{
  size_t size = 0;
  int primes [] = {2, 3, 5, 7, 11, 13, 17, 19};
  // Initial size
  seq[size] = 1;
  size++;

  // Add in powers of primes
  for (int j = 0; j < sizeof (primes) / sizeof (primes[0]); j++)
    for (int i = primes[j]; i <= MAX_SIZE; i *= primes[j])
      {
	seq[size] = i;
	size++;
      }

  qsort (seq, size, sizeof (seq[0]), &cmpint);
  return size;
}

// Initialize memory to random values
void
mem_init_random (uint8_t *ptr, size_t len)
{
  for (size_t i = 0; i < len; i++)
    ptr[i] = rand () % 256;
}

// Initialize memory to a constant value
void
mem_init_const (uint8_t *ptr, size_t len, uint8_t val)
{
  for (size_t i = 0; i < len; i++)
    ptr[i] = val;
}

// Validate memory against a reference
void
mem_validate_ref (uint8_t *ptr, uint8_t *ref_ptr, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (ptr[i] != ref_ptr[i])
      printf ("mem_validate_ref: expected 0x%02x, got 0x%02x at offset %u\n",
	      ref_ptr[i], ptr[i], i);
}

// Validate memory against a value
void
mem_validate_val (uint8_t *ptr, uint8_t ref_val, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (ptr[i] != ref_val)
      printf ("mem_validate_val: expected 0x%02x, got 0x%02x at offset %u\n",
	      ref_val, ptr[i], i);
}

// Get the cycle count
uint64_t ccount (void)
{
  uint64_t cycles;
  __asm__ volatile ("rdcycle\t%0" : "=r"(cycles));
  return cycles;
}

// Subtract two values. Even if we have wrapped round, it will work.
uint64_t
cdiff (uint64_t cstart, uint64_t cend)
{
  uint64_t cycles = cend - cstart;

  if (cend < cstart)
    {
      fprintf (stderr,
	       "Warning: cdiff: backwards time: %llu -> %llu, zero assumed\n",
	       cstart, cend);
	cycles = 0;
    }

  return cycles;
}

// Run one overhead calculation
uint64_t
run_bm (uint64_t (*bm_fn) (size_t), size_t len, uint64_t coverhead)
{
  uint64_t res = 0;
  for (size_t i = 0; i < WARMUP; i++)
    res += bm_fn (len);

  res = 0;
  for (size_t i = 0; i < REPEATS; i++)
    res += bm_fn (len);

  return res / REPEATS - coverhead;
}

// Benchmark function for computing overhead
uint64_t
run_overhead (size_t len __attribute__ ((unused)) )
{
  uint64_t cstart = ccount ();
  uint64_t cend = ccount ();
  return cdiff (cstart, cend);
}

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
