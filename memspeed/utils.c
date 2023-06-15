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

/// Initialize memory to random values
///
/// \param[out] ptr  The memory to initialize
/// \param[in] len   The size of the memory in bytes
void
mem_init_random (uint8_t *ptr, size_t len)
{
  for (size_t i = 0; i < len; i++)
    ptr[i] = rand () % 256;
}

// Initialize memory to a constant value
///
/// \param[out] ptr  The memory to initialize
/// \param[in] len   The size of the memory in bytes
/// \param[in] val   The value to use for initialization
void
mem_init_const (uint8_t *ptr, size_t len, uint8_t val)
{
  for (size_t i = 0; i < len; i++)
    ptr[i] = val;
}

/// Check for memory equivalence
///
/// \param[in] v1   First vector to compare
/// \param[in] v2   Second vector to compare
/// \param[in] len  Length of the vectors
/// \return  The total |v1[i] - v2[i]| for the whole vector
uint64_t
mem_cmp (uint8_t *v1, uint8_t *v2, size_t len)
{
  uint64_t res = 0;

  for (size_t i = 0; i < len; i++)
    res += abs (v1[i] - v2[i]);

  return res;
}

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
