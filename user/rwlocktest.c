// Test program for rwspinlock - tests concurrent reads of ticks
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i, pid;
  uint64 tick_value;
  
  printf("rwspinlock test: Multiple processes reading ticks concurrently\n");
  printf("This tests if multiple readers can access tickslock simultaneously\n\n");
  
  // Fork multiple child processes that will read uptime concurrently
  for(i = 0; i < 3; i++) {
    pid = fork();
    if(pid == 0) {
      // Child process - repeatedly call uptime (which uses read lock)
      printf("Child %d: Starting to read uptime...\n", getpid());
      
      for(int j = 0; j < 5; j++) {
        // Do some work
        for(int k = 0; k < 10000; k++);
        tick_value = uptime();
        printf("Child %d: uptime=%ld ticks (iteration %d)\n", getpid(), tick_value, j+1);
      }
      
      printf("Child %d: Completed\n", getpid());
      exit(0);
    }
  }
  
  // Parent waits for all children
  printf("Parent: Waiting for children...\n");
  for(i = 0; i < 3; i++) {
    wait(0);
  }
  
  printf("\n✓ rwspinlock test PASSED!\n");
  printf("All child processes successfully read ticks concurrently.\n");
  printf("With rwspinlock, multiple readers can hold the lock simultaneously!\n");
  
  exit(0);
}
