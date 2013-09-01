#!/bin/sh
#/sbin/ifconfig -a |grep HWaddr | cut -c 39-55 |tr -d : >/root/logs/mac.txt
/root/lemote_line/tools/eeprom/read_mac | tr -d : > /root/logs/mac.txt

