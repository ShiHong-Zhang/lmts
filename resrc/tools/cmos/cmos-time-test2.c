#include <stdio.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

FILE *p_file = NULL;
int i, j, fd, retval, irqcount = 0;
unsigned long tmp, data;
struct rtc_time rtc_tm, rtc_tm1, rtc_tm2;
int ret = 0;


int restore_rtc( struct rtc_time rtc_tm )
{
        /* Set the RTC time/date, and print it out */
	retval = ioctl(fd, RTC_SET_TIME, &rtc_tm);
	if (retval == -1) {
		perror("ioctl");
		exit(errno);
        }

        return 0;
}


int main(void) 
{

	if ((p_file = fopen("./rtc_data.txt", "w")) == NULL ) {
		fprintf(stderr, "Error. Can not creat rtc_data.txt.\n" );
		return -1;
	}

	/* open the rtc device */
	fd = open ("/dev/rtc", O_RDWR);
	if (fd == -1) {
		perror("/dev/rtc");
		exit(errno);
        }

	/* read the rtc time first, and print it out */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (retval == -1) {
                perror("ioctl");
                exit(errno);
        }

  	fprintf( stderr, "%d-%d-%d, %02d:%02d:  %02d.\n",
    	rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	/* prepare the rtc time struct variable, set the rtc time manually */
	rtc_tm1.tm_mday = 8;
	rtc_tm1.tm_mon = 3 - 1;
	rtc_tm1.tm_year = 2009 - 1900;
	rtc_tm1.tm_hour = 0;
	rtc_tm1.tm_min = 0;
	rtc_tm1.tm_sec = 0;

	/* Set the RTC time/date, and print it out */
	retval = ioctl(fd, RTC_SET_TIME, &rtc_tm1);
	if (retval == -1) {
		perror("ioctl");
		exit(errno);
        }

        fprintf( stderr, "%d-%d-%d, %02d:%02d:  %02d.\n",
    	        rtc_tm1.tm_mday, rtc_tm1.tm_mon + 1, rtc_tm1.tm_year + 1900,
		rtc_tm1.tm_hour, rtc_tm1.tm_min, rtc_tm1.tm_sec);

	sleep(5);

	/* read it again */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm2);
	if (retval == -1) {
		perror("ioctl");
		exit(errno);
        }

	/* compare between the just read value and the value written */
	if (rtc_tm2.tm_mday == rtc_tm1.tm_mday
                && rtc_tm2.tm_mon == rtc_tm1.tm_mon
                && rtc_tm2.tm_year == rtc_tm1.tm_year
		&& rtc_tm2.tm_hour == rtc_tm1.tm_hour
		&& rtc_tm2.tm_min  == rtc_tm1.tm_min
		&& rtc_tm2.tm_sec - rtc_tm1.tm_sec == 5) {
		
		fprintf(stderr, "Check OK!\n");
		ret = 0;

	}
	else {
		fprintf(stderr, "Check Error!\n");
		/* if error, print the read value */
		fprintf( stderr, "%d-%d-%d, %02d:%02d:  %02d.\n",
    		        rtc_tm2.tm_mday, rtc_tm2.tm_mon + 1, rtc_tm2.tm_year + 1900,
			rtc_tm2.tm_hour, rtc_tm2.tm_min, rtc_tm2.tm_sec);
    	
		ret = -1;
	}
	
	/* restore the original rtc time */
	restore_rtc( rtc_tm );
	
    	close(fd);
    	fclose(p_file);
        return ret;
}

