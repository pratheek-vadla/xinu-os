#ifndef _HEAP_H
#define _HEAP_H

#define MINHEAP 128


typedef struct _heap_t {
  uint32 size;
  char* startaddr;
  char* freelist;
} heap_t;

struct memBlock {
  struct memBlock *next;
  struct memBlock *prev;
  uint32 size;
};

extern void initheap(char*, uint32);
extern syscall getheap(pid32, uint32);
extern syscall freeheap(pid32);
extern	pid32	create2(void *, uint32, uint32, pri16, char *, uint32, ...);

extern void* malloc(uint32);
extern void free(char*, uint32);
extern sid32 sync_malloc;
#endif
