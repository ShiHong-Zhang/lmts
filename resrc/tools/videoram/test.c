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

#define TST_PATTERN1 0xA53C5AC3

	
int main( int argc, char *argv[] )
{
	int fd, i, j, tmp=0;
	int retval = 0;	
	struct timeval tv1, tv2;
	
	
		gettimeofday( &tv1, NULL);
		
		for (j = 0; j < 100; j++) {
   		/* Compare writing and reading data */
   		for (i = 0; i < 1000000; i += 4) {
			tmp = tmp + 1;
		}
		}
		gettimeofday( &tv2, NULL);
		printf("The Videoram writing rate is: %f M\n", 40.0 / ((tv2.tv_sec - tv1.tv_sec) + 
							(tv2.tv_usec - tv1.tv_usec)*0.000001));

	return 0;
}

