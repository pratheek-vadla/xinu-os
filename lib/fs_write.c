#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * Copy 'len' bytes from 'buff' into the blocks associated with the file 'fd'
 *   - Until 'len' bytes are copied...
 *   - Check if a new block is needed (find free block and add to the file's blocks if necessary)
 *   - Write bytes from the 'buff' into the block starting at the fileptr
 *   - Write 'len' bytes or fill the block, whichever is less
 *   - Repeat until write is complete
 *   - Update inode state to reflect changes
 *   - Return the number of bytes written
 */
int fs_write(int fd, char* buff, int len) {
	
	inode_t *inode = &oft[fd].in;
	int blockIdx;
	if(inode->size > fsd->device.blocksz){	
		blockIdx = inode->size / fsd->device.blocksz - 1;// + (inode->size % fsd->device.blocksz == 0 ? 0 : 1);
	}else{
		blockIdx = 0;
	}
	//printf("Starting at BlockIdx: %d\n", blockIdx);
	/*
	if(inode->size % fsd->device.blocksz == 0){
		blockIdx=0; 
		// Need New Block
		for(int i=0; i < fsd->freemasksz; i++){
              		if(fs_getmaskbit(i) == 0){
                        	inode->blocks[blockIdx] = i;
                                fs_setmaskbit(i);
                                break;
                         }
                }
	}*/
	int avail_mem = 0;
	int temp_buff = 0;
	int bytes_written = 0;
	while(len > 0){
		//printf("========================================\n");
		if(inode->size == 0){
			for(int i=0; i < fsd->freemasksz; i++){
                                if(fs_getmaskbit(i) == 0){
                                        inode->blocks[blockIdx] = i;
                                        fs_setmaskbit(i);
                                        break;
                                }
                        }
                        avail_mem = fsd->device.blocksz;
                        //printf("Initialised First Block at BlockIdx: %d\n", blockIdx);
		}
		else if(inode->size % fsd->device.blocksz == 0){
			blockIdx+=1;
			// Need New Block
                	for(int i=0; i < fsd->freemasksz; i++){
                        	if(fs_getmaskbit(i) == 0){
                                	inode->blocks[blockIdx] = i;
                                	fs_setmaskbit(i);
                                	break;
                         	}
                	}
			avail_mem = fsd->device.blocksz;
			//printf("New Block Created, BlockIdx: %d\n", blockIdx);
        	}else{
			avail_mem = fsd->device.blocksz - inode->size % fsd->device.blocksz;
			//printf("Writing in current block, BlockIdx: %d\n", blockIdx);
		}

		if(len > avail_mem){
			temp_buff = avail_mem;
		}else{
			temp_buff = len;
		}

		//printf("Writing %d bytes at (fileptr): %d\n", temp_buff, oft[fd].fileptr);
		bs_write(inode->blocks[blockIdx], oft[fd].fileptr, &buff[bytes_written], temp_buff);

		len -= temp_buff;
		bytes_written += temp_buff;
		inode->size += temp_buff;
		oft[fd].fileptr = (oft[fd].fileptr + temp_buff) % fsd->device.blocksz;
	}
	//fs_print_fd(fd);
	return bytes_written;
}
