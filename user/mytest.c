#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "kernel/memlayout.h"

int
main(int argc, char *argv[])
{
  printf("=== Testing USYSCALL Implementation ===\n\n");
  
  // Method 1: Use the regular getpid() syscall
  printf("Method 1: Using getpid() system call\n");
  int pid1 = getpid();
  printf("  PID from syscall: %d\n\n", pid1);
  
  // Method 2: Use the optimized ugetpid() - no syscall!
  printf("Method 2: Using ugetpid() - direct memory read\n");
  int pid2 = ugetpid();
  printf("  PID from USYSCALL page: %d\n\n", pid2);
  
  // Method 3: Manually read from the USYSCALL page
  printf("Method 3: Manually reading from USYSCALL page\n");
  struct usyscall *u = (struct usyscall *)USYSCALL;
  printf("  USYSCALL virtual address: 0x%lx\n", USYSCALL);
  printf("  PID from manual read: %d\n\n", u->pid);
  
  // Verify they all match
  if (pid1 == pid2 && pid2 == u->pid) {
    printf("✓ SUCCESS: All three methods return the same PID!\n");
    printf("  Regular syscall: %d\n", pid1);
    printf("  Optimized ugetpid(): %d\n", pid2);
    printf("  Manual USYSCALL read: %d\n", u->pid);
  } else {
    printf("✗ FAILURE: PIDs don't match!\n");
    printf("  syscall=%d, ugetpid=%d, manual=%d\n", pid1, pid2, u->pid);
  }
  
  printf("\n=== Testing in child process ===\n");
  int child_pid = fork();
  
  if (child_pid == 0) {
    // Child process
    printf("\nChild process:\n");
    printf("  getpid() = %d\n", getpid());
    printf("  ugetpid() = %d\n", ugetpid());
    printf("  USYSCALL page PID = %d\n", ((struct usyscall *)USYSCALL)->pid);
    exit(0);
  } else {
    // Parent process
    wait(0);
    printf("\nParent process:\n");
    printf("  getpid() = %d\n", getpid());
    printf("  ugetpid() = %d\n", ugetpid());
    printf("  USYSCALL page PID = %d\n", ((struct usyscall *)USYSCALL)->pid);
  }
  
  printf("\n=== Performance Test ===\n");
  printf("Calling getpid() 10000 times with syscall...\n");
int i;
uint64 start, end, syscall_time, nosyscall_time;

start = uptime();
for (i = 0; i < 10000; i++) {
    getpid();
}
end = uptime();
syscall_time = end - start;
printf("Done with syscalls! Time: %ld ticks\n", syscall_time);

printf("Calling ugetpid() 10000 times without syscall...\n");
start = uptime();
for (i = 0; i < 10000; i++) {
    ugetpid();
}
end = uptime();
nosyscall_time = end - start;
printf("Done without syscalls! Time: %ld ticks\n", nosyscall_time);
if (nosyscall_time > 0) {
    printf("Speedup: %ldx faster\n", syscall_time / nosyscall_time);
} else {
    printf("No-syscall version too fast to measure!\n");
}
  
  printf("\n=== Test Complete ===\n");
  exit(0);
}
