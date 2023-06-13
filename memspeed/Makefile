# Makefile for the example.

# This file is part the RVV memcpy/memset evaluation

# Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

# SPDX-License-Identifier: GPL-3.0-or-later

QEMU = qemu-riscv64
QEMU_SARGS = -cpu rv64,Zicsr=true
QEMU_VARGS = -cpu rv64,v=true,Zfh=true,Zicsr=true
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-gcc
SCFLAGS = -march=rv64gc_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
VCFLAGS = -march=rv64gcv_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
ASFLAGS = -march=rv64gcv_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
LDFLAGS =

SOBJS = smain.o smem-asm.o smem-static.o utils.o
VOBJS = vmain.o smem-asm.o smem-vector.o utils.o vmem.o
SEXE = demo-static
VEXE = demo-vector

.PHONY: all
all: $(SEXE) $(VEXE)
	@$(QEMU) $(QEMU_SARGS) $(SEXE)
	@$(QEMU) $(QEMU_VARGS) $(VEXE)

$(SEXE): $(SOBJS)
	$(CC) $(SCFLAGS) $(LDFLAGS) $^ -o $@

$(VEXE): $(VOBJS)
	$(CC) $(VCFLAGS) $(LDFLAGS) $^ -o $@

smain.o: smain.c utils.h
	$(CC) $(SCFLAGS) -c $< -o $@

smem-asm.o: smem-asm.S
	$(AS) $(ASFLAGS) -c $< -o $@

smem-static.o: smem-static.c
	$(CC) $(SCFLAGS) -c $< -o $@

smem-vector.o: smem-vector.c
	$(CC) $(VCFLAGS) -c $< -o $@

utils.o: utils.c utils.h
	$(CC) $(SCFLAGS) -c $< -o $@

vmain.o: vmain.c utils.h
	$(CC) $(VCFLAGS) -c $< -o $@

vmem.o: vmem.S
	$(AS) $(ASFLAGS) -c $< -o $@

smem-static.c: smem.c
	cp $^ $@

smem-vector.c: smem.c
	cp $^ $@

.PHONY: clean
clean:
	$(RM) $(SOBJS) $(VOBJS) $(SEXE) $(VEXE)
	$(RM) *.s *.i
	$(RM) smem-static.c smem-vector.c
