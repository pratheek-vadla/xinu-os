#include <future.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
	intmask mask;
	mask = disable();

	// TODO: write your code here
  	if (mode == FUTURE_EXCLUSIVE){
		
		future_t* f = getmem(sizeof(future_t));
        	f->mode = mode;
        	f->size = size;
		// f->pid = getpid();
        	f->state = FUTURE_EMPTY;
        	f->data = getmem(size);
        	restore(mask);
        	return f;	
	}else{
		// NOT IMPLEMENTED
		restore(mask);
		return NULL;
	}
	
}

syscall future_free(future_t* f){
	
	if(f == NULL){
		printf("Empty future provided! Cannot free from memory.\n");
		return SYSERR;
	}
	kill((int)f->pid);
	freemem(f->data, sizeof(f->size));
	freemem(f, sizeof(future_t));
}

syscall future_get(future_t* f, void* out){
	
	
	if(f->state == FUTURE_EMPTY){
		
		f->state = FUTURE_WAITING;
		f->pid = getpid();
		suspend(f->pid);
		memcpy(out, f->data, f->size);
		return OK;

	}else if(f->state == FUTURE_WAITING){
		
		return SYSERR;

	}else if(f->state == FUTURE_READY){
		
		f->state = FUTURE_EMPTY;
		memcpy(out, f->data, f->size);
		return OK;

	}else{
		
		return SYSERR;

	}

}

syscall future_set(future_t* f, void* out){

	if(f->state == FUTURE_EMPTY){
		memcpy(f->data, out, f->size);
                f->state = FUTURE_READY;
                return OK;
        }else if(f->state == FUTURE_WAITING){
		memcpy(f->data, out, f->size);
		f->state = FUTURE_EMPTY;	
		resume(f->pid);
		return OK;
	}else{
                return SYSERR;
        }
}
