# Makefile for the example.

# This file is part the RVV memcpy/memset evaluation

# Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

# SPDX-License-Identifier: GPL-3.0-or-later

# Host tools
HOST_CC = gcc
HOST_CFLAGS = -ggdb3 -O2
HOST_LIBS = -lm

# Target tools
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-gcc
SCFLAGS = -march=rv64gc_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
VCFLAGS = -march=rv64gcv_zfh_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
ASFLAGS = -march=rv64gcv_zfh_zicsr -mabi=lp64d -ggdb3 -O2 -save-temps
LDFLAGS =

# Object files for each target executable
SSOBJS = ssmain.o smem-scalar.o utils.o
SVOBJS = svmain.o smem-vector.o utils.o
VVOBJS = vvmain.o utils.o vmem-asm.o

# The target executables
SSEXE = demo-standard-scalar
SVEXE = demo-standard-vector
VVEXE = demo-vector-vector

# Host executable
EXE=gensizes
SRC=gensizes.c

.PHONY: all
all: $(EXE) $(SSEXE) $(SVEXE) $(VVEXE)

$(EXE): $(SRC)
	$(HOST_CC) $(HOST_CFLAGS) $^ $(HOST_LIBS) -o $@

$(SSEXE): $(SSOBJS)
	$(CC) $(SCFLAGS) $(LDFLAGS) $^ -o $@

$(SVEXE): $(SVOBJS)
	$(CC) $(VCFLAGS) $(LDFLAGS) $^ -o $@

$(VVEXE): $(VVOBJS)
	$(CC) $(VCFLAGS) $(LDFLAGS) $^ -o $@

# The different main programs
ssmain.o: ssmain.c utils.h
	$(CC) $(SCFLAGS) -c $< -o $@

svmain.o: svmain.c utils.h
	$(CC) $(VCFLAGS) -c $< -o $@

vvmain.o: vvmain.c utils.h
	$(CC) -DVECTOR $(VCFLAGS) -c $< -o $@

# The difference memcpy versions
smem-scalar.o: smem-scalar.c
	$(CC) $(SCFLAGS) -c $< -o $@

smem-vector.o: smem-vector.c
	$(CC) $(VCFLAGS) -c $< -o $@

vmem-asm.o: vmem-asm.S
	$(AS) $(ASFLAGS) -c $< -o $@

# Utils
utils.o: utils.c utils.h
	$(CC) $(SCFLAGS) -c $< -o $@

# Generated source
ssmain.c: main.c
	cp $^ $@

svmain.c: main.c
	cp $^ $@

vvmain.c: main.c
	cp $^ $@

smem-scalar.c: smem.c
	cp $^ $@

smem-vector.c: smem.c
	cp $^ $@

.PHONY: clean
clean:
	$(RM) $(SSOBJS) $(SVOBJS) $(VVOBJS)
	$(RM) $(EXE) $(SSEXE) $(SVEXE) $(VVEXE)
	$(RM) *.s *.i
	$(RM) smem-scalar.c smem-vector.c ssmain.c svmain.c vvmain.c
