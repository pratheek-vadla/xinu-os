#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * fs_open - Return SYSERR if the file is already open or does not exist
 *     otherwise, add a record to the oft in the first current FSTATE_CLOSED
 *     slot corresponding to the opened file and copy the inode from the block
 *     device to the oft record and return the index into the oft table for the
 *     newly opened file.
 */
int fs_open(char* filename, int flags) {
	
	// Check if file is already open.
	for(int i=0; i < NUM_FD; i++){

		if(strcmp(fsd->root_dir.entry[oft[i].de].name, filename) == 0){
			
			if(oft[i].state == FSTATE_OPEN){
				return SYSERR;
			}
		}	
	}
	
	int num_entries = fsd->root_dir.numentries;
	for(int i=0; i < num_entries; i++){
		if(strcmp(fsd->root_dir.entry[i].name, filename) == 0){
                        inode_t file;
                        bs_read(fsd->root_dir.entry[i].inode_block, 0, &file, sizeof(inode_t));
			for(int j=0; i < NUM_FD; i++){
					
                        	if(oft[j].state == FSTATE_CLOSED){
                                		
					oft[j].state = FSTATE_OPEN;
    					oft[j].de = i;
    					oft[j].flag = flags;
					oft[j].in = file;
					oft[j].fileptr = 0;
					return j;
                        	}
        		}
                }
	}
	return SYSERR;
}
