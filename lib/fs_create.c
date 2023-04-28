#include <xinu.h>
#include <fs.h>
#include <string.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * fs_create - Return SYSERR if not enough space is available
 *     in the directory or the filename already exists.
 *     Otherwise:
 *        1. Find an available block on the block store
 *        2. Create an inode_t for the new file
 *        3. Update the root directory
 *        4. Write the inode and free bitmask back to the block device
 */
syscall fs_create(char* filename) {
	
	int num_entries = fsd->root_dir.numentries;
	if(num_entries == DIR_SIZE){
		return SYSERR;
	}
	for(int i=0; i < num_entries; i++){
		if(strcmp(fsd->root_dir.entry[i].name, filename) == 0){
			//printf("Duplicate Filename!\n");
			return SYSERR;
		}
	}

	int masksize = fsd->freemasksz;
	for (int i=0; i < masksize; i++){
		if(fs_getmaskbit(i) == 0){
			// 1. Found Available Block at Index: i
			// 2. Create inode_t
			inode_t newfile;
			newfile.id = i;
			newfile.size = 0;
			// 3. Update root dir
			int idx = fsd->root_dir.numentries;
			fsd->root_dir.entry[idx].inode_block = i;
			strcpy(fsd->root_dir.entry[idx].name, filename);
			fsd->root_dir.numentries += 1;
			// 4. Writing to the block device
			bs_write(i, 0, &newfile, sizeof(inode_t));
			fs_setmaskbit(i);
			return OK;
		}
	}
	return SYSERR;
}

