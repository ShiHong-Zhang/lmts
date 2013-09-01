#!/bin/bash

setterm -blank 0
echo "memtest start......."
#sleep 10

if [ -f "/root/logs/prememtester_failed" ]; then

	echo "Will you test memory again?(y/n)"
	read -n 1 ch
	if [ ch != "y" || ch != "Y" ]; then
		echo	
        echo "==================================================== "
        echo 
        echo "########    ###    #### ##       ######## ########  "
        echo "##         ## ##    ##  ##       ##       ##     ## "
        echo "##        ##   ##   ##  ##       ##       ##     ## "
        echo "######   ##     ##  ##  ##       ######   ##     ## "
        echo "##       #########  ##  ##       ##       ##     ## "
        echo "##       ##     ##  ##  ##       ##       ##     ## "
        echo "##       ##     ## #### ######## ######## ########  "
        echo
        echo "==================================================== "
        echo
        echo "Error reason: Memory data comparison failed."
        echo 
        cat /root/logs/prememtester-log.txt
        echo
        echo "Please press ENTER key to continue..."
        echo

        sleep 100000000000000000000000
		read -n 1
	else
        rm -rf /root/logs/prememtester_failed
	fi                
fi

memtester="/root/lemote_line/tools/ime_memtester/memtester"

if [ ! -f "$memtester" ]; then

	echo "$memtester not found." >> /root/logs/prememtester-log.txt
        echo "Error time:" `date` >> /root/logs/prememtester-log.txt

	echo "$memtester not found."
        echo
        echo "Please press ENTER key to continue..."
        echo
	read -n 1
	exit 1
else
	echo "$memtester"
	$memtester 450 2 2>/root/logs/prememtester-log.txt 
fi	

ret=$?
if [ $ret -eq "0" ]; then
        touch /root/logs/prememtester_pass
        # show pass
	        
        echo
        echo
        echo "================================================ "
        echo ""
        echo "        ########      ###      ######    ######  "
        echo "        ##     ##    ## ##    ##    ##  ##    ## "
        echo "        ##     ##   ##   ##   ##        ##       "
        echo "        ########   ##     ##   ######    ######  "
        echo "        ##         #########        ##        ## "
        echo "        ##         ##     ##  ##    ##  ##    ## "
        echo "        ##         ##     ##   ######    ######  "
        echo ""
        echo "================================================ "
        echo
        echo "Please wait for 3 seconds ..."
        echo
        
        sleep 3
        
        rm -f /etc/rc2.d/S10memtester.sh
        
else
        # show Failed
        touch /root/logs/prememtester_failed
        
        echo "Error reason: Memory data comparison failed." >> /root/logs/prememtester-log.txt
        echo "Error time:" `date` >> /root/logs/prememtester-log.txt
        echo
        echo	
        echo "==================================================== "
        echo 
        echo "########    ###    #### ##       ######## ########  "
        echo "##         ## ##    ##  ##       ##       ##     ## "
        echo "##        ##   ##   ##  ##       ##       ##     ## "
        echo "######   ##     ##  ##  ##       ######   ##     ## "
        echo "##       #########  ##  ##       ##       ##     ## "
        echo "##       ##     ##  ##  ##       ##       ##     ## "
        echo "##       ##     ## #### ######## ######## ########  "
        echo
        echo "==================================================== "
        echo
        echo "Error reason: Memory data comparison failed."
        echo
        echo "Please press ENTER key to continue..."
        echo

        sleep 100000000000000000000000                                                                                                                                                                                                                                                                                                                                
        read -n 1 
fi

exit $?
