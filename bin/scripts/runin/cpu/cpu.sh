#!/bin/sh

if [ -z $CPU ]; then
	CPU="/root/lmts/resrc/tools/cputester/cputester"
fi

ret=`$CPU 1>> /root/logs/cputester-log.txt 2>&1`

return $ret
