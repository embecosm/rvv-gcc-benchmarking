// Test program for scalar or vector memcpy performance.

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define TRUE  1
#define FALSE 0
#include "utils.h"

#ifdef VECTOR
extern void *vmemcpy(void *__restrict dest, const void *__restrict src,
		     size_t n);
#else
extern void *smemcpy(void *__restrict dest, const void *__restrict src,
		     size_t n);
#endif

/// Run a scalar memcpy.
///
/// We give the option to run with our without the memcpy to allow
/// differential timing.
///
/// \param[in] argc  The number of arguments, including the command itself
///                  (which should be 3)
/// \param[in] argv  The vector of arguments. First is the size of copy, the
///                  second is whether this is a dummy run.
int
main (int argc, char* argv[])
{
  if (argc != 3)
    {
      fprintf (stderr, "Usage: %s <size> dummy|real\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  size_t  len = strtoull (argv[1], NULL, 0);
  if (len == 0)
    {
      sprintf (stderr, "ERROR: size must be non-zero\n");
      exit (EXIT_FAILURE);
    }

  int is_real = (0 == strcasecmp ("real", argv[2])) ? TRUE : FALSE;

  uint8_t *src = (uint8_t *) malloc (len);
  uint8_t *dst = (uint8_t *) malloc (len);
  mem_init_random (src, len);
  mem_init_const (dst, len, 0);

  if (TRUE == is_real)
#ifdef VECTOR
    vmemcpy (dst, src, len);
#else
    smemcpy (dst, src, len);
#endif

  return (mem_cmp (dst, src, len) == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
