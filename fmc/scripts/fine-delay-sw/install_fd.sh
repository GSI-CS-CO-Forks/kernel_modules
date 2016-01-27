#!/bin/sh

TRANSFER=/etc/transfer.ref
DEVICE_NAME=FMC-FDELAY
DRIVER_NAME=fmc-fine-delay

# symlink to true FDELAY="fmc/svec-fine-delay-<date>-<version>.bin"
FDELAY="fmc/svec-fine-delay.bin"

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

echo "Installing zio driver ..."
(cat /proc/modules | grep -q '^zio ' ) || (cd ../zio/ ; sh install_zio.sh)

# this is superfluous; driver installation priorities take care of
# this pre-requisite, and when spec+fdelay is required, it is the
# fmc bus who actually knows what to install
(cat /proc/modules | grep -q '^svec ' ) || (cd ../svec/ ; sh install_svec.sh)

echo "Installing fine delay driver..."
/sbin/insmod $DRIVER_NAME.ko gateware=$FDELAY verbose=1

# FIXME: fd-luns.awk is VME-dependent; entirely different for SPEC
echo "$DRIVER_NAME: making device nodes"
awk -f ./fd-luns.awk $DEVICE_NAME $TRANSFER | sh

( cd ../wr-core ; python bootstrap-wr-core.py -t $TRANSFER $DEVICE_NAME )
