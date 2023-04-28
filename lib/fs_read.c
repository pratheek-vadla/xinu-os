#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * Copy the data from the block device into the 'buff' argument
 *    Use the fileptr field of the oft to track the location of the next read/write
 *    operation.
 *    - Determine the current block and offset from the fileptr
 *    - Copy bytes into the 'buff' until you have exhausted the block or copied 'len' bytes
 *    - Repeat until 'len' bytes are copied
 *    - Update the fileptr field
 *    - Return the number of bytes read
 */
int fs_read(int fd, char* buff, int len) {
	
	//printf("Length: %d\n", len);
	
	int blockIdx = oft[fd].fileptr / fsd->device.blocksz;
	int offset;
	int bytes_read = 0;
	int bytes_to_read = 0;
	int remaining_bytes_in_curr_block = 0;
	
	while(len > 0){
		offset = oft[fd].fileptr % fsd->device.blocksz;
		//printf("Current BlockIdx: %d, Offset: %d\n", blockIdx, offset);
		remaining_bytes_in_curr_block = fsd->device.blocksz - offset;
		//printf("Remaining Bytes in Curr Block: %d\n", remaining_bytes_in_curr_block);
		
		if(len < remaining_bytes_in_curr_block){
			bytes_to_read = len;
		}else{
			bytes_to_read = remaining_bytes_in_curr_block;
		}
		//printf("Reading: %d bytes from offset: %d from blockIdx: %d\n", bytes_to_read,offset, blockIdx);
		bs_read(oft[fd].in.blocks[blockIdx], offset, buff, bytes_to_read);
			
		//printf("Before: %d\n", oft[fd].fileptr);
		oft[fd].fileptr += bytes_to_read;
		//printf("After: %d\n", oft[fd].fileptr);
		
		bytes_read += bytes_to_read;
		
		len -= bytes_to_read;
		//printf("oft[fd].fileptr: %d\n", oft[fd].fileptr);
	
		if(bytes_read % fsd->device.blocksz == 0){
			blockIdx += 1;
			//printf("\n====================\nMoving to next block: %d\n", blockIdx);
		}
		buff = &(buff[bytes_read]);	
	}
  	return bytes_read;
}
