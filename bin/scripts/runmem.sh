#!/bin/bash -e

mount -o remount,rw /
mount -t proc proc /proc

/root/lmts/bin/scripts/memtester.sh
wait

umount /proc
ln -sf loonux /sbin/LOonux3
sync
#shutdown -rn now

exec /sbin/loonux
