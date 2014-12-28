#!/bin/sh

dd if=/dev/zero of=yzzfs bs=1K count=1000
mkfs.ext2 yzzfs
gcc 0498chmn.c -o chmn && ./chmn && rm chmn

mkdir /mnt/yzzfs

echo "---- mounting yzzfs.new as yzzext2 to /mnt/yzzfs"
mount -t yzzext2 -o loop yzzfs.new /mnt/yzzfs
echo "-- greped mount, yzzfs info"
mount | grep yzz

# echo "---- try to mknod a fifo file"
# cd /mnt/yzzfs
# mknod yzz_fifo p
# echo '-- dmesg | tail -n 1'
# dmesg | tail -n 1

sleep 1
umount -f /mnt/yzzfs
echo "---- /mnt/yzzfs unmounted"

echo "mounting yzzfs.new as ext2 to /mnt/yzzfs"
mount -t ext2 -o loop yzzfs.new /mnt/yzzfs
