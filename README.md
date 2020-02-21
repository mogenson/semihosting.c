# semihosting.c

A raw semihosting file IO implimentation for stdin, stdout, and stderr to be used with ARM Cortex-M microcontrollers. Just add the `semihosting.c` source file to your compilation list and the `_read()` and `_write()` stubs will redirect normal `stdio.h` functions like `printf()` and `scanf()` to the semihosting host.
