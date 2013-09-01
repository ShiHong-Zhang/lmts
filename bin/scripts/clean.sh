#!/bin/bash

cd /root
echo "==========================================="
echo "Begin to clean"

#if [ -f /etc/sudoers_orig ]; then
 #   mv /etc/sudoers_orig /etc/sudoers
#fi

#rm /etc/xdg/autostart/manufactrue_test.desktop

rm -rf /root/lmts*
rm -rf /root/logs/
rm -rf /root/tmp/

rm /tmp/*  1>/dev/null 2>&1
echo "Cleaning Successfully."

rm /root/.bash_history -rf
rm /home/loongson/.bash_history -rf
history -c

#reboot
