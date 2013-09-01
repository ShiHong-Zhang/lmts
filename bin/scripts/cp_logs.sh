#!/bin/bash

if [ $# -ne 1 ];then
	echo "use $0 mach_ID."
	exit 0
fi

disk_num=`sudo fdisk -l | grep 'Disk /dev/' | wc -l`

if [ $disk_num -lt 2 ]; then
	echo "************请检查U盘是否插好*************"
	sleep 3
	exit
elif [ $disk_num -eq 2 ]; then
	disk=`sudo fdisk -l | grep 'Disk /dev/' | tail -n 1 |awk -F':' '{print $1}' | awk '{print $2}'`
elif [ $disk_num -gt 2 ]; then
	disk=`sudo fdisk -l | grep 'Disk /dev/' | head -n 2 | tail -n 1 |awk -F':' '{print $1}' | awk '{print $2}'`
fi


echo disk : $disk
echo ""

usb=`sudo fdisk -l | grep ${disk} | head -n 2 | tail -n 1 | awk '{print $1}'`

echo usb : $usb
echo ""

sudo umount ${disk}* 1>/dev/null 2>&1
sudo mount ${usb} /mnt 1>/dev/null 2>&1

sudo cp -r  /root/logs  /mnt/$1_logs
sync

if [   -d /mnt/$1_logs  ] ;then
echo "******************************************"
echo "                Finish copying            "
echo "******************************************"
echo ""
else
echo "******************************************"
echo "                Copy failed               "
echo "******************************************"
echo ""
fi
echo "now I'll umount the Usb Flash Disk......."
echo ""

sudo umount ${usb}

if [ $? -ne 0 ]; then
	echo "******************************************"
	echo "            Err: Umount failed            "
	echo "******************************************"
	echo ""
else
	echo "******************************************"
	echo "            OK! Finish umounting          "
	echo "******************************************"
	echo ""
fi

