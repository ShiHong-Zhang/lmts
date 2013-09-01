#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define LOOP 5

void display_usage()
{
	printf ("usage: videoram start_addr end_addr\n");
	printf ("\n");
	exit (0);
}
int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	if ((ch = getopt(argc, argv, "h")) != -1)
	{
		printf("videoram unkown option: -%c\n", ch);
		display_usage();
	}
	
	if (argc != 3)
	  display_usage();

	int addr[2], i;
	char *endpstr;
	for (i = 1; i < argc; ++i)
	{
		endpstr = '\0';
		addr[i-1] = strtol(argv[i], &endpstr, 16);
		if (*endpstr)
		{
			fprintf(stderr, "No.%d arg: %s to long Failed.\n", i, argv[i]);
			exit(1);
		}
	}

	int fd;
	struct timeval t1, t2;
	char *data;
	void *base;
	int size = addr[1] - addr[0] + 1;
	float slice, speed;

	if ((fd = open("/dev/mem", O_RDWR)) < 0) {
		perror("open");
		return -1;
	}

	base = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr[0]);
	if (base == NULL) {
		perror("mmap");
		return -2;
	}
	
	data = (char *)malloc(size);
	bzero(data, size);
	gettimeofday(&t1, NULL);
	for (i=0; i<LOOP; i++)
		memcpy(base, data, size);
	gettimeofday(&t2, NULL);
	slice = (t2.tv_sec - t1.tv_sec) + (float)(t2.tv_usec - t1.tv_usec) / (float)1000000;
	speed = (float)size * LOOP / slice;

	printf("Copy Time: %fs \nCopye Size: %d MB \nSpeed: %.8f MB/s\n", slice, LOOP*size/1024/1024, speed / (float)1024 / (float)1024);
	munmap(base, size);
	close(fd);
	
	return 0;
}
