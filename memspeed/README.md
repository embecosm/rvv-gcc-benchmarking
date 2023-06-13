# Evaluation of RISC-V vector memcpy/memset performance

We wish to compare the standard newlib memcpy and memset against RVV versions
of these functions, in both case optimized for speed.

## Running the tests

Ensure you have `riscv64-unknown-elf-gcc` and `riscv64-qemu` on your search
path.  Then run
```
make all
```
