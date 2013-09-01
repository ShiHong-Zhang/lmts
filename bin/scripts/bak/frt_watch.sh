#!/bin/bash
check(){
	cd /root/lemote_line/scripts/
	bash check.sh $1 $2
	cd -
}

i=1
while [ 1 ]
do
	check runin $i
	i=`expr $i + 1`
	sleep 1800
done
