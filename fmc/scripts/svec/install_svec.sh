#!/bin/sh

TRANSFER=/etc/transfer.ref
DEVICE_NAME=FMC-SVEC-A24
DRIVER_NAME=svec

# symlink to current golden bitstream GOLDEN="fmc/golden-svec-<date>-<version>.bin"
GOLDEN="fmc/svec-golden.bin"

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
INSMOD_ARGS=`awk -f ./svec-modparams.awk $DEVICE_NAME $TRANSFER |
	sed 's!\$GOLDEN!'$GOLDEN'!g'`
echo "svec: insmod with $INSMOD_ARGS"

$RUN /sbin/insmod $DRIVER_NAME.ko $INSMOD_ARGS
