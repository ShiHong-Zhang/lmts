#!/bin/bash

FILE="/root/logs/times.txt"
LOGFILE="/root/logs/reboot_log.txt"


if [ ! -z $1 ]; then
	times=$1
else
        times=100
fi

if [ ! -e ${FILE} ]; then
	echo 1 > $FILE
	echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" >> $LOGFILE
	echo -e ">> 1\t`date +"%y-%m-%d %H:%M:%S"`" >> $LOGFILE
	sleep 3
	reboot
	sleep 36000000
fi

T=`cat ${FILE}`
if [ $T -lt "$times" ]; then
	T=`expr $T + 1`
	echo "Reboot times: $T"
	echo $T > $FILE
	echo -e ">> $T\t`date +"%y-%m-%d %H:%M:%S"`" >> $LOGFILE
	echo I will reboot in 3 seconds...
	sleep 3
	reboot
	sleep 36000000
fi
echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" >> $LOGFILE
