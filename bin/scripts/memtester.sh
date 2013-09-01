#!/bin/bash

setterm -blank 0
echo "memtest start......."

if [ -f "/root/logs/MEM-test-failed" ]; then
	echo	
	echo -e "\033[31m" "==================================================== "
	echo 
	echo -e "\033[31m" "########    ###    #### ##       ######## ########  "
	echo -e "\033[31m" "##         ## ##    ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##        ##   ##   ##  ##       ##       ##     ## "
	echo -e "\033[31m" "######   ##     ##  ##  ##       ######   ##     ## "
	echo -e "\033[31m" "##       #########  ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##       ##     ##  ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##       ##     ## #### ######## ######## ########  "
	echo
	echo -e "\033[31m" "==================================================== "
	echo
	echo "Error reason: Memory data comparison failed."
	echo 
	cat /root/logs/prememtester-log.txt
	echo
	echo "Please press ENTER key to continue..."
	echo

	read -n 1 
fi

memtester="/root/lmts/resrc/tools/memtester/memtester"
meminfo=`cat /proc/meminfo | head -n 1 | awk '{print $2}'`

if [ ! -f "$memtester" ]; then

	echo "$memtester not found." >> /root/logs/prememtester-log.txt
	echo "Error time:" `date` >> /root/logs/prememtester-log.txt
	touch /root/logs/flags/MEM-test-failed

	echo "$memtester not found."
	echo
	echo "Please press ENTER key to continue..."
	echo
	read -n 1
	exit 1
else
	freemem=`free -m | grep 'Mem' | awk '{print $4}'`
	testmem=`expr ${freemem} - 10`
	$memtester ${testmem} 2 | tee /root/logs/prememtester-log.txt
#	$memtester 4 1 1 | tee /root/logs/prememtester-log.txt
#	$memtester 4 1 1>>/root/logs/prememtester-log.txt

#	echo "$memtester"
#	if [ $(((1024-50)*1024)) -lt $meminfo ] && [ $(((1024+50)*1024)) -gt $meminfo ]; then
#		echo -e "Start\t: `date +"%y-%m-%d %H:%M:%S"`\t$memtester 900 2 2" |tee /root/logs/prememtester-log.txt
#		$memtester 900 2 2>>/root/logs/prememtester-log.txt
#	elif [ $(((512-50)*1024)) -lt $meminfo ] && [ $(((512+50)*1024)) -gt $meminfo ]; then
#		echo -e "Start: `date +"%y-%m-%d %H:%M:%S"`\t$memtester 450 2 2" |tee /root/logs/prememtester-log.txt
#		$memtester 450 2 2>>/root/logs/prememtester-log.txt
#	elif [ $(((2048-50)*1024)) -lt $meminfo ] && [ $(((2048+50)*1024)) -gt $meminfo ]; then
#		echo -e "Start: `date +"%y-%m-%d %H:%M:%S"`\t$memtester 1800 1 2" |tee /root/logs/prememtester-log.txt
#		$memtester 1800 1 2>>/root/logs/prememtester-log.txt
#	fi
fi	

ret=$?
if [ $ret -eq "0" ]; then
	touch /root/logs/flags/MEM-test-pass
	echo -e "End\t: `date +"%y-%m-%d %H:%M:%S"`" >>/root/logs/prememtester-log.txt
	# show pass

	echo
	echo
	echo -e "\033[32m" "================================================ "
	echo ""
	echo -e "\033[32m" "        ########      ###      ######    ######  "
	echo -e "\033[32m" "        ##     ##    ## ##    ##    ##  ##    ## "
	echo -e "\033[32m" "        ##     ##   ##   ##   ##        ##       "
	echo -e "\033[32m" "        ########   ##     ##   ######    ######  "
	echo -e "\033[32m" "        ##         #########        ##        ## "
	echo -e "\033[32m" "        ##         ##     ##  ##    ##  ##    ## "
	echo -e "\033[32m" "        ##         ##     ##   ######    ######  "
	echo ""
	echo -e "\033[32m" "================================================ "
	echo
	echo "Please wait for 3 seconds ..."
	echo
	
	sleep 3
else
	# show Failed
	touch /root/logs/flags/MEM-test-failed
	
	echo "Error reason: Memory data comparison failed." >> /root/logs/prememtester-log.txt
	echo "Error time:" `date` >> /root/logs/prememtester-log.txt
	echo
	echo	
	echo -e "\033[31m" "==================================================== "
	echo 
	echo -e "\033[31m" "########    ###    #### ##       ######## ########  "
	echo -e "\033[31m" "##         ## ##    ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##        ##   ##   ##  ##       ##       ##     ## "
	echo -e "\033[31m" "######   ##     ##  ##  ##       ######   ##     ## "
	echo -e "\033[31m" "##       #########  ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##       ##     ##  ##  ##       ##       ##     ## "
	echo -e "\033[31m" "##       ##     ## #### ######## ######## ########  "
	echo
	echo -e "\033[31m" "==================================================== "
	echo
	echo "Error reason: Memory data comparison failed."
	echo
	echo "Please press ENTER key to continue..."
	echo

	read -n 1 
fi
exit 0
