#!/bin/bash
set -e

kdecf=~yee/.kde/share/config
kdeap=~yee/.kde/share/apps
inidir=./inifile
xlog=/root/logs/xlog

ini_to_chk=(
	"${kdecf}/kickerrc" "${inidir}/kickerrc.ini" "任务栏变宽"
	"${kdecf}/kdeglobals" "${inidir}/kdeglobals.ini" "字体变小"
	"${kdecf}/kdesktoprc" "${inidir}/kdesktoprc.ini" "壁纸出错"
	"${kdeap}/kdesktop/IconPositions" "${inidir}/IconPositions9003.ini"  "桌面图标排列出错"
	~yee/.kderc "${inidir}/kderc.ini" "自定义字体出错"
)

cfg_path=""
example=""
comment=""


#get logs
dmesg > ${xlog}/dmesg_log_$1_$2.txt.tmp
sync
mv ${xlog}/dmesg_log_$1_$2.txt.tmp ${xlog}/dmesg_log_$1_$2.$(date +%F-%H-%M-%S).txt

cat /home/yee/.xsession-errors > ${xlog}/xsession_log_$1_$2.txt.tmp
sync
mv ${xlog}/xsession_log_$1_$2.txt.tmp ${xlog}/xsession_log_$1_$2.$(date +%F-%H-%M-%S).txt

cat /var/log/Xorg.0.log > ${xlog}/xorg_log_$1_$2.txt.tmp
sync
mv ${xlog}/xorg_log_$1_$2.txt.tmp ${xlog}/xorg_log_$1_$2.$(date +%F-%H-%M-%S).txt


#check target file
for i in "${ini_to_chk[@]}"
do
	if [ -z "$cfg_path" ]; then
		cfg_path="$i"
	elif [ -z "$example" ]; then
		example="$i"
	else
		comment="$i"

		if python chk_ini.py "$cfg_path" "$example"; then
			echo ""
		else
			cp $cfg_path ${xlog}/$(basename ${cfg_path})_error_log_$1_$2.txt.tmp
			sync
			mv ${xlog}/$(basename ${cfg_path})_error_log_$1_$2.txt.tmp ${xlog}/$(basename ${cfg_path})_error_log_$1_$2.$(date +%F-%H-%M-%S).txt

			echo ""
			echo "$cfg_path ERROR"
			echo ""
			echo "$cfg_path ERROR"
			echo ""
			echo "$cfg_path ERROR"

			kdialog --msgbox "$comment，请告知研发人员。" 2>/dev/null || true
			exit 2
		fi

		cfg_path=""
		example=""
		comment=""
	fi
done
