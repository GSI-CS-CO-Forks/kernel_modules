#!/bin/sh

DEVICE_NAME=CVORB
DRIVER_NAME=cvorb
TRANSFER=/etc/transfer.ref
DRIVER_VERSION=

OUTPUT=":"
RUN=""

KERNEL=`uname -s`
VERSION=`uname -r`

# if called with CVORB as first argument, drop them all
if [ x"$1" = x"$DEVICE_NAME" ] ; then
    while [ x"$*" != x"" ] ; do
	shift
    done
fi

while getopts hvnc:D:d:t: o
do	case $o in
	v)	OUTPUT="echo" ;;		# verbose
	n)	RUN=":" ;;			# dry run
	D)	DEVICE_NAME="$OPTARG" ;;
	d)	DRIVER_NAME="$OPTARG" ;;
	t)	TRANSFER="$OPTARG" ;;
	[h?])	echo >&2 "usage: $0 [-?hvn] [-D device_name] [-d driver_name] [-c crateconfig]"
		exit ;;
	esac
done

$OUTPUT "Installing $DEVICE_NAME driver..."
INSMOD_ARGS=`awk -f $DRIVER_NAME.awk $DEVICE_NAME $CRATECONFIG $TRANSFER`
if [ x"$INSMOD_ARGS" == x"" ] ; then
    echo "No $DEVICE_NAME declared in $TRANSFER, exiting"
    exit 1
fi

# manage exception
HOSTNAME=$(hostname)
if [ x"$DRIVER_VERSION" = x"" -a -r $HOSTNAME ] ; then
	DRIVER_VERSION=`cat $HOSTNAME`
	echo "installing special version $DRIVER_VERSION for $HOSTNAME"
	DRIVER_VERSION=-$DRIVER_VERSION
fi

INSMOD_CMD="/sbin/insmod $DRIVER_NAME.ko$DRIVER_VERSION $INSMOD_ARGS"
$OUTPUT "$DRIVER_NAME install by [$INSMOD_CMD]"
sh -c "$RUN $INSMOD_CMD"

MAJOR=`cat /proc/devices | awk '$2 == "'"$DRIVER_NAME"'" {print $1}'`
if [ -z "$MAJOR" ]; then
	echo "driver $DRIVER_NAME not installed!"
	exit 1
fi

# change device node permission (default is 66- )
sleep 1
chmod 0666 /dev/$DRIVER_NAME.[0-9]*
