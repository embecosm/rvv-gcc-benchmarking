// RVV assembler implementations of memcpy and memset

// This file is part the RVV memcpy/memset evaluation

// Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// SPDX-License-Identifier: GPL-3.0-or-later

	.file	"vmem.S"
	.option arch, +v
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1

	// Reference implementation from the RVV standard of vector memcpy
	// void *memcpy(void* dest, const void* src, size_t n)
	// a0 = dest, a1 = src, a2 = n
	//
memcpy:
	.globl	vmemcpy
	.type	vmemcpy, @function
vmemcpy:
	mv	a3, a0			// Copy destination
loop_cpy:
	vsetvli	t0, a2, e8, m8, ta, ma	// Vectors of 8b
	vle8.v	v0, (a1)		// Load bytes
	add	a1, a1, t0		// Bump pointer
	sub	a2, a2, t0		// Decrement count
	vse8.v	v0, (a3)		// Store bytes
	add	a3, a3, t0		// Bump pointer
	bnez	a2, loop_cpy		// Any more?
	ret
	.size	vmemcpy, .-vmemcpy

	// Simple implementation of vector vmemset
	// void *memset(void *s, int c, size_t n);
	// a0 = s, a1 = c, a2 = n
	.globl	vmemset
	.type	vmemset, @function
vmemset:
	mv	a3, a0			// Copy destination
	vsetvli	t0, a2, e8, m8, ta, ma  // Vectors of 8b
	vmv.v.x	v0, a1			// Copy constant values into source
loop_set:
	vsetvli	t0, a2, e8, m8, ta, ma  // Vectors of 8b
	sub	a2, a2, t0		// Decrement count
	vse8.v	v0, (a3)		// Store constant bytes
	add	a3, a3, t0		// Bump pointer
	bnez	a2, loop_set		// Any more?
	ret
	.size	vmemset, .-vmemset

// Local Variables:
// mode: assembler
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
