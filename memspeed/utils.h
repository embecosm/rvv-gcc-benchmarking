// Header for utility functions

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

// Number of repeats of an action before measuring
#define WARMUP 10

// Number of repeats of an action when measuring
#define REPEATS 100

// Largest data size
#define MAX_SIZE (1024*1024)

size_t genseq (int  seq[]);
void mem_init_random (uint8_t *ptr, size_t len);
void mem_init_const (uint8_t *ptr, size_t len, uint8_t val);
void mem_validate_ref (uint8_t *ptr, uint8_t *ref_ptr, size_t len);
void mem_validate_val (uint8_t *ptr, uint8_t ref_val, size_t len);
uint64_t ccount (void);
uint64_t cdiff (uint64_t cstart, uint64_t cend);
uint64_t run_bm (uint64_t (*bm_fn) (size_t), size_t len, uint64_t coverhead);
uint64_t run_overhead (size_t len __attribute__ ((unused)) );

#endif	// UTILS_H

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
