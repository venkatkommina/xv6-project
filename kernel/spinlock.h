// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

#ifdef LAB_LOCK
// Read-write spinlock.
struct rwspinlock {
  struct spinlock lock;  // Base spinlock for mutual exclusion
  int readers;           // Number of active readers
  int writer;            // Is a writer active? (0 or 1)
  int writer_waiting;    // Is a writer waiting? (for priority)
  
  // For debugging:
  char *name;            // Name of lock
};
#endif

