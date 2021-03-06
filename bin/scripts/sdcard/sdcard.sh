#!/bin/bash
# ===========================================
# This bash script is for 3ANB sdcard test

#=============
#return value
#=============
OK=0
NO_SD_DEV=1
UMOUNT_ERR=2
MOUNT_ERR=3
DIFF_ERR=4


MOUNT_POINT="/mnt"
SD_TEST_FILE="/root/tmp/sd_test_bag.tar.gz"


# ===========================================
# find sdcard device
# ===========================================

SD_DEV_LINK="/dev/disk/by-id/usb-Generic-_Multi-Card_20071114173400000-0:0-part1"
SD_DEV_NAME=`readlink -e ${SD_DEV_LINK}`
if [ ! -b ${SD_DEV_NAME} ]; then
	echo "have not Sdcard Device"
	exit ${NO_SD_DEV}
fi

# ===========================================
# check sdcard and mount point
# ===========================================

IS_SD_DEV_MOUNT=`mount | grep ${SD_DEV_NAME}`
if [ -z '${IS_SD_DEV_MOUNT}' ]; then
	echo "sdcard has not mounted"
else
	umount ${SD_DEV_NAME}
	if [ $? -ne 0 ]; then
		echo "umount device failed"
		exit ${UMOUNT_ERR}
	fi
fi

IS_MOUNT_POINT_USED=`mount | grep ${MOUNT_POINT}`
if [ -z ${IS_MOUNT_POINT_USED} ]; then
	echo "mount point is not used"
else
	umount ${MOUNT_POINT}
	if [ $? -ne 0 ]; then
		echo "umount device failed"
		exit ${UMOUNT_ERR}
	fi
fi


# ===========================================
# mount sdcard to mnt
# ===========================================
mount ${SD_DEV_NAME} ${MOUNT_POINT}
if [ $? -ne 0 ]; then
	echo "mount sdcard failed"
	exit ${MOUNT_ERR}
fi


# ===========================================
# prepare oprea file
# ===========================================
tar czf ${SD_TEST_FILE} /usr/include

# ===========================================
# copy to sdcard and diff and remove
# ===========================================

cp ${SD_TEST_FILE} ${MOUNT_POINT}/testbag_1.img
cp ${SD_TEST_FILE} ${MOUNT_POINT}/testbag_2.img
sync

diff ${MOUNT_POINT}/testbag_1.img ${MOUNT_POINT}/testbag_2.img
if [ $? -ne 0 ]; then
	echo "diff error"
	exit ${DIFF_ERR}
fi

rm ${SD_TEST_FILE} -rf
rm ${MOUNT_POINT}/testbag_1.img ${MOUNT_POINT}/testbag_2.img -rf

exit ${OK}
