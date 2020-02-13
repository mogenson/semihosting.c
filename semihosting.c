extern int _write(int file, char *buf, int len); // declared elsewhere in libc
extern int _read(int file, char *buf, int len);  // declared elsewhere in libc

/* inline function with inline assembly to exchange buffers with debugger */
static inline int syscall(int op, void *arg) {
  int volatile ret;
  asm volatile("mov  r0, %[op]\n"  // move op into r0 register
               "mov  r1, %[arg]\n" // move arg into r1 register
               "bkpt 0xAB\n"       // call debugger (in Thumb mode)
               "mov  %[ret], r0\n" // move result out of r0 register
               : [ ret ] "=r"(ret) // ret is an asm variable output to C
               : [ op ] "r"(op), [ arg ] "r"(arg) // op and arg are asm inputs
               : "r0", "r1"); // tell GCC r0 and r1 will be modified
  return ret;
}

/* returns number of bytes written or -1 on error */
int _write(int file, char *buf, int len) {
  /* anonymous 3 word array with file handler, pointer to buffer, and length */
  int ret = syscall(0x05, (void *)[]{(void *)file, buf, (void *)len}); // write
  return (ret < 0) ? -1 : len - ret; // ret is number of bytes not written
}

/* returns number of bytes read or -1 on error */
int _read(int file, char *buf, int len) {
  /* anonymous 3 word array with file handler, pointer to buffer, and length */
  int ret = syscall(0x06, (void *)[]{(void *)file, buf, (void *)len}); // read
  return (ret < 0) ? -1 : len - ret; // ret is number of bytes not read
}
