// Header for utility functions

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

void mem_init_random (uint8_t *ptr, size_t len);
void mem_init_const (uint8_t *ptr, size_t len, uint8_t val);
uint64_t mem_cmp (uint8_t *v1, uint8_t *v2, size_t len);

#endif	// UTILS_H

// Local Variables:
// mode: C
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
