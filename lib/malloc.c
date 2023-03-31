#include <xinu.h>
#include <heap.h>

extern heap_t heaptab[];
void initheap(char* startaddr, uint32 size) {
  /* This function is called when the heap is constructed */
  /*   Use as needed to setup the free list               */
  struct memBlock* mb = (struct memBlock*) startaddr;
  mb->size = size;
  mb->next = NULL;
  mb->prev = NULL;
  //printf("============================\n");
  //printf("Start Address: %d\n", startaddr);
  //printf("============================\n");
  return;
}

void* malloc(uint32 size) {
  /* Implement memory allocation within the process heap here       */
  /*   Your implementation MUST NOT use getmem                      */
  /*   Your implementation SHOULD use explicit in-memory free lists */
	pid32 pid = getpid();
  	if (heaptab[pid].size < size){
	//	printf("Err: Low Storage!\n");
      		return (char*)SYSERR;
  	}
	// Initialising and Traversing through Free List for available memory
  	struct memBlock* currMemblk = (struct memBlock*) heaptab[pid].freelist;
  	uint32 fsize = currMemblk->size;
  	while (size > fsize){
	//	printf("Searching..\n");
  		currMemblk = currMemblk->next;
		if(currMemblk == NULL){
	    		return (char*)SYSERR;
		}
		fsize = currMemblk->size;
  	}
	// If requested Memory block is available:
	// 1. Update free memory block
	char* new_addr = (char *) currMemblk + size; // New Start Addr for Free List
	struct memBlock* mb = (struct memBlock*) new_addr;
  	mb->size = (uint32) (currMemblk->size - size);
  	mb->next = NULL;
	mb->prev = currMemblk; // Setting prev pointer to the currently allocated block
	/*	
	printf("============================\n");
	printf("New Free List:\n");
	printf("Start Addr: %d\n", new_addr);
	printf("Size: %d\n", mb->size);
	printf("Prev Addr: %d\n", (char *)currMemblk);
	*/
	// Point the free list head to the updated address
	heaptab[pid].freelist = (char *) mb;
	heaptab[pid].size -= size;
	heaptab[pid].startaddr = new_addr;
	
 	// Update allocated blocks prev, next, size values
  	currMemblk->size = size;
  	currMemblk->next = mb;
	/*
	printf("==\n");
  	printf("Allocated Block:\n");
	printf("Start Addr: %d\n", (char *)currMemblk);
	printf("Size: %d\n", currMemblk->size);	
	printf("Next: %d\n", (char *)currMemblk->next);
	printf("Prev: %d\n", (char *)currMemblk->prev);
	printf("============================\n");
	*/
	return (void*)currMemblk;
}

void coalese_fragments(){
	// Coalesing adjacent blocks in free list
	pid32 pid = getpid();
        struct memBlock* head = (struct memBlock*) heaptab[pid].freelist;
	while(head->next != NULL){
		char* tail = (char *) head + head->size;
		struct memBlock* next_block = (struct memBlock*) head->next;
		if(tail == (char*) next_block){
			head->next = next_block->next;
			head->size = head->size + next_block->size;
		}
		head = head->next;
	}
}

void free(char* block, uint32 size) {
  /* Implement memory free within process heap here */
  /*   Your implementation MUST implement coalesing */

	struct memBlock* currBlk = (struct memBlock*) block; // block to be freed
	currBlk->size = size;
	
	pid32 pid = getpid();
	struct memBlock* head = (struct memBlock*) heaptab[pid].freelist;
	char* tail = (char *) head + head->size;
	if(block >= tail){
		while (block > tail){
                	head = head->next;
        	}
        	struct memBlock* next_mem_blk = head->next;
        	head->next = currBlk;
        	currBlk->next = next_mem_blk;
	}else{
		char* block_tail = (char *) block + size;
		if (block_tail < (char *) head){
			// Insert at the front of free list
			currBlk->next = head;
			heaptab[pid].freelist = block;
		}
	}
	heaptab[pid].size += size;	
	coalese_fragments();
  return;
}
