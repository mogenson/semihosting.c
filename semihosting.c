extern int _read(int file, char* buf, int len);   // declared elsewhere in libc
extern int _write(int file, char* buf, int len);  // declared elsewhere in libc
void __attribute__((constructor)) init(void);     // called in __libc_init_array

static int fds[3] = {0, 1, 2};  // file descriptors for stdin, stdout, stderr

/* inline function with inline assembly to exchange buffers with debugger */
static inline int syscall(int op, void* arg) {
    int volatile ret;
    asm volatile(
        "mov  r0, %[op]\n"   // move op into r0 register
        "mov  r1, %[arg]\n"  // move arg into r1 register
        "bkpt 0xAB\n"        // call debugger (in Thumb mode)
        "mov  %[ret], r0\n"  // move result out of r0 register
        : [ ret ] "=r"(ret)  // ret is an asm variable output to C
        : [ op ] "r"(op), [ arg ] "r"(arg)  // op and arg are asm inputs
        : "r0", "r1");  // tell GCC r0 and r1 will be modified
    return ret;
}

/* open file descriptors for stdin, stdout, and stderr */
void init() {
    for (int i = 0; i < 3; i++)  // anonymous array with magic file name
        fds[i] = syscall(0x01, (int[]){(int)":tt", 4 * i, 3});  // sys_open
}

/* returns number of bytes written or -1 on error */
int _write(int file, char* buf, int len) {
    if (file != 1 && file != 2) return -1;  // must be stdout or stderr
    int ret = syscall(0x05, (int[]){fds[file], (int)buf, len});  // sys_write
    return ret < 0 ? -1 : len - ret;  // ret is number of bytes not written
}

/* return number of bytes read or -1 on error */
int _read(int file, char* buf, int len) {
    if (file) return -1;  // must be stdin
    int ret = syscall(0x06, (int[]){fds[file], (int)buf, len});  // sys_read
    return ret < 0 ? -1 : len - ret;  // ret is number of bytes not read
}
