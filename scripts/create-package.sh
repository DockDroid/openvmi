#!/bin/bash

set -ex

ramdisk=$1
system=$2

if [ -z "$ramdisk" ] || [ -z "$system" ]; then
	echo "Usage: $0 <ramdisk> <system image>"
	exit 1
fi

workdir=`mktemp -d`
rootfs=$workdir/rootfs

mkdir -p $rootfs

# Extract ramdisk and preserve ownership of files
(cd $rootfs ; cat $ramdisk | gzip -d | sudo cpio -i)

mkdir $workdir/system
sudo mount -o loop,rw $system $workdir/system
sudo cp -ar $workdir/system/* $rootfs/system
sudo umount $workdir/system

# FIXME
sudo chmod +x $rootfs/openvmi-init.sh

if [ -e android.img ]; then
    DATE=`date +%F_%R`
    SAVETO=android-old-$DATE.img

    echo "#########################################################"
    echo "# WARNING: Old android.img still exists.                 "
    echo "#          Moving it to $SAVETO.                         "
    echo "#########################################################"

    mv android.img $SAVETO
fi

sudo mksquashfs $rootfs android.img -comp xz -no-xattrs
sudo chown $USER:$USER android.img

sudo rm -rf $workdir
