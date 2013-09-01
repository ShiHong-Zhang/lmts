#!/bin/bash

sleep 15
exit 0
#

HOME_DIR="/root/lmts/"
LOG_DIR="/root/logs"
FLAG_DIR="/root/logs/flags"
TMP_DIR="/root/tmp"

if [ ! -d $LOG_DIR ]; then
	mkdir -p $LOG_DIR
	chmod -R 777 $LOG_DIR
fi

if [ ! -d $FLAG_DIR ]; then
	mkdir -p $FLAG_DIR
	chmod -R 777  $FLAG_DIR
fi

if [ ! -d $TMP_DIR ]; then
	mkdir -p $TMP_DIR
	chmod -R 777 $TMP_DIR
fi

cd $HOME_DIR

# for ntp and checkntp, stop local ntpd
/etc/init.d/ntp stop

if [ -z $BURNIN_TIMES ]; then
	BURNIN_TIMES=1
fi

if [ -z $REBOOT_TIMES1 ]; then
	REBOOT_TIMES1=50
fi

if [ -z $REBOOT_TIMES2 ]; then
	REBOOT_TIMES2=20
fi

if [ -z $NB3A_SMT_DIR ]; then
	NB3A_DIR="mach_type/notebook/smt/"
fi

if [ -z $NB3A_FAT_DIR ]; then
	NB3A_DIR="mach_type/notebook/fat/"
fi

if [ -z $NB3A_FRT_DIR ]; then
	NB3A_DIR="mach_type/notebook/frt/"
fi

if [ -z $NB3A_FFT_DIR ]; then
	NB3A_DIR="mach_type/notebook/fft/"
fi

if [ -z "$LMTS" ]; then
	LMTS=lmts
fi

#SMT
#while [ ! -f "$FLAG_DIR/function-smt-pass" ]; do

	#3anb SMT

#	cd mach_types/notebook/smt
#	./$LMTS
#	wait
#	cd ../../../
#	cd $HOME_DIR
#done

#if [ -f "$FLAG_DIR/function-smt-pass" ]; then
#	rm -rf $FLAG_DIR/function-smt-pass
#	poweroff   
#fi

#########################################################
#FAT
while [ ! -f "$FLAG_DIR/function-fat-pass" ]; do
	cd mach_types/notebook/fat
	./$LMTS
	wait
	cd $HOME_DIR
done

if [ ! -f "$FLAG_DIR/burnin_enable" ] && [ ! -f "$FLAG_DIR/burnin_disable" ]; then
	gdialog --title "烧机" --yesno "是否需要烧机"

	if [ $? -eq 0 ]; then 
		touch $FLAG_DIR/burnin_enable
	else
		touch $FLAG_DIR/burnin_disable
	fi
fi

#memtester  
if [ -f "$FLAG_DIR/burnin_enable" ]; then
	while [ ! -f "$FLAG_DIR/MEM-test-pass" ]; do
		rm /sbin/LOonux3 && ln -sf /root/lmts/bin/scripts/runmem.sh /sbin/LOonux3
		poweroff
	done

#reboot 30
	if [ ! -f "$FLAG_DIR/function-frt-pass" ]; then
		cd bin/scripts/
		./do_reboot.sh $REBOOT_TIMES1
		cd $HOME_DIR
#runin
		cd mach_types/notebook/frt
		./$LMTS
		ret=$?
		cd $HOME_DIR
		sleep 1
	
		if [ $ret -ne "0" ]; then
			touch $FLAG_DIR/function-frt-failed
			echo "Error: unnormally exited from burnin." >> $LOG_DIR/log.txt
			echo `date` >> $LOG_DIR/log.txt
			exit 1
		fi
		
		#################################################

#reboot 20		
		cd bin/scripts/
		./do_reboot.sh $REBOOT_TIMES2
		cd $HOME_PATH
		sleep 1
	fi
fi

#FFT

while [ ! -f "$FLAG_DIR/function-fft-pass" ]; do
	cd mach_types/notebook/fft
	./$LMTS
	wait
	cd $HOME_PATH
done

# ========================================
# clear
if [ -e "$FLAG_DIR/function-fft-pass" ]; then
	cd bin
	./scripts/clean.sh
fi
