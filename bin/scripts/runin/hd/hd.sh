#!/bin/bash


DIR="/root/tmp/"
LOGDIR="/root/logs/"
ORIG=${DIR}linux.orig
i=0
LOG_FILE="/root/logs/log.txt"
HDCOPY_FILE="/root/logs/hd_copy_log.txt"
DATE_CMD="date +%y-%m-%d~~%H:%M:%S"

echo start: `$DATE_CMD` >> $LOG_FILE
rm -rf /root/tmp/linux* 


while [ 1 ]; do

echo $i
echo $ORIG
echo going to tar
tar cf  ${ORIG}  /usr/bin /lib /usr/share/doc 1>/dev/null 2>&1 
sync; sleep 1
ret=$?
if [ $ret -ne 0 ]; then
	echo tar Error Time: `$DATE_CMD` >> $LOG_FILE
	echo tar Error Time: `$DATE_CMD` >> $HDCOPY_FILE
	echo tar error
	exit 1
fi

cp -avf ${ORIG} ${DIR}linux.new${i}
sync; sleep 1
ret=$?
if [ $ret -ne 0 ]; then
	echo cp Error Time: `$DATE_CMD` >> $LOG_FILE
	echo cp Error Time: `$DATE_CMD` >> $HDCOPY_FILE
	echo cp error
	exit 1
fi

echo going to diff
diff ${ORIG} ${DIR}linux.new${i}
sync; sleep 1
ret=$?
if [ $ret -ne 0 ]; then
	echo diff Error Time: `$DATE_CMD` >> $LOG_FILE
	echo diff Error Time: `$DATE_CMD` >> $HDCOPY_FILE
	echo diff error
	exit 1
fi

echo going to rm
rm -f ${ORIG}
rm -f ${DIR}linux.new${i}
ret=$?
if [ $ret -ne 0 ]; then
	echo rm Error Time: `$DATE_CMD` >> $LOG_FILE
	echo rm Error Time: `$DATE_CMD` >> $HDCOPY_FILE
	echo rm error
	exit 1
fi

let i=i+1

done

