/* 
 * videoram_test.c
 * we allocate a big block of video memory (as large as total video ram) space to check  
 * 
 * tanggang -- 080121
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

//#define BLOCK_SIZE	0x200000			// 2M
//#define MEMORY_SIZE	0x400000			// 4M  SM712 MEM RAM capacity
//#define BLOCK_SIZE	0x100000			// 1M
#define BLOCK_SIZE	0x10000			// 1M
#define MEMORY_SIZE	0x200000			// 2M  SM501 MEM RAM capacity
#define START_ADDR_OFF	0x0			// 0

#define TST_PATTERN1 0xA53C5AC3

	
int main( int argc, char *argv[] )
{
	int fd, i, j, tmp;
	struct stat sb;
	char *region, *src;
	int retval = 0;	
	char *pfile = "/dev/fb0";
	/*
	char fbfile[2][10] = {"/dev/fb1", "/dev/fb0"};
	
	for (i = 0; i < 2; ++i)
		if (access(fbfile[i], F_OK) == 0)
		{
			pfile = fbfile[i];
			break;
		}
	*/
	if ((fd = open(pfile, O_RDWR)) < 0) {
		perror("open");
		return -1;
	}
	
	/* here, we will repeat for four times */
//	for (i = 0; i < (MEMORY_SIZE - START_ADDR_OFF)/BLOCK_SIZE; i += 1) {
		printf ("line: %d\n", __LINE__);
		region = mmap( 	NULL,
				BLOCK_SIZE, 
				PROT_READ|PROT_WRITE, 
				MAP_SHARED, 
				fd, 
				(unsigned int)(START_ADDR_OFF + BLOCK_SIZE) );

		printf ("line: %d\n", __LINE__);

		if (region == MAP_FAILED ) {
			perror("mmap");
			return -1;
		}
//		printf( "i  %d  region %x -- value %x\n", i, region,*(int *)region);
		/*
		if( mlock((void *)region, BLOCK_SIZE) ) {
			perror("mlock");
			return -1;
		}
		*/
		
		printf ("line: %d\n", __LINE__);
   		/* Compare writing and reading data */
		int *base = (int *)region;
   		//for (j = 0; j < BLOCK_SIZE; j += sizeof(int)) {
		printf ("size: %d\n", BLOCK_SIZE / 1024);

   		for (j = 0; j < BLOCK_SIZE / 4; j++ ) {
			*(base + j) = TST_PATTERN1;
			usleep (8000);
			tmp = *(base + j);
			//printf( "j %d  write: %x  read : %x\n", j, TST_PATTERN1, tmp);
			usleep (1000);
			if (tmp != TST_PATTERN1) {
				printf( "Reading data is different form writing data.\n");
				printf( "j %d  write: %x  read : %x\n", j, TST_PATTERN1, tmp);
				return -1;
			}
		}

		printf ("line: %d\n", __LINE__);
		munmap( region, BLOCK_SIZE );
	//}

	close(fd);
	return 0;
}

