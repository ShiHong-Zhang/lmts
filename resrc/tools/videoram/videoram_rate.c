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

#define BLOCK_SIZE	0x200000			// 2M
#define MEMORY_SIZE	0x400000			// 4M  SM712 MEM RAM capacity
#define START_ADDR_OFF	0x0			// 0

#define TST_PATTERN1 0xA53C5AC3

	
int main( int argc, char *argv[] )
{
	int fd, i, j, tmp;
	char *region, *src;
	int retval = 0;	
	struct timeval tv1, tv2;
	FILE *fp;
	
	if ((fp = fopen("rate_value.txt", "w")) == NULL) {
		printf("Error when write content to rate_value.txt\n");
		return -1;
	}
	
	if ((fd = open("/dev/fb0", O_RDWR)) < 0) {
		perror("open");
		return -1;
	}
	
	/* here, we will repeat for four times */
		region = mmap( 	NULL,
				BLOCK_SIZE, 
				PROT_READ|PROT_WRITE, 
				MAP_SHARED, 
				fd, 
				(unsigned int)(START_ADDR_OFF + BLOCK_SIZE) );

		if (region == (void *)-1 ) {
			perror("mmap");
			return -1;
		}
		
		/*
		if( mlock((void *)region, BLOCK_SIZE) ) {
			perror("mlock");
			return -1;
		}
		*/
		gettimeofday( &tv1, NULL);
		
		for (j = 0; j < 15; j++) {
   		/* Compare writing and reading data */
   		for (i = 0; i < BLOCK_SIZE; i += 4) {
			*(unsigned int *)(region + i) = TST_PATTERN1;
		}
		}
		gettimeofday( &tv2, NULL);
		fprintf( fp, "Rate: %.2f M\n", 30.0 / ((tv2.tv_sec - tv1.tv_sec) + 
							(tv2.tv_usec - tv1.tv_usec)*0.000001));
		munmap( region, BLOCK_SIZE );

	close(fd);
	fclose(fp);
	return 0;
}

