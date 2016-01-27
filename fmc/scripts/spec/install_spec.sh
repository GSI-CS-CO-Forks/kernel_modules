#!/bin/sh

TRANSFER=/etc/transfer.ref
DEVICE_NAME=FMC-SPEC
DRIVER_NAME=spec

# symlink to current golden bitstream GOLDEN="fmc/spec-init.bin
GOLDEN="fmc/spec-init.bin"

OUTPUT=":"
RUN=""

while getopts hvn:D:d:t: o
do	case $o in
	v)	OUTPUT="echo" ;;		# verbose
	n)	RUN=":" ;;			# dry run
	D)	DEVICE_NAME="$OPTARG" ;;
	d)	DRIVER_NAME="$OPTARG" ;;
	t)	TRANSFER="$OPTARG" ;;
	[h?])	echo >&2 "usage: $0 [-?hvn] [-D device_name] [-d driver_name] [-t transfer]"
		exit ;;
	esac
done

$OUTPUT "Installing fmc-bus driver"
(cat /proc/modules | grep -q "^fmc ") || /sbin/insmod ../fmc-bus/fmc.ko

$OUTPUT "$DRIVER_NAME install"

$RUN /sbin/insmod $DRIVER_NAME.ko fw_name=$GOLDEN
