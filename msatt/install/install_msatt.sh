#!/bin/sh

CRATECONFIG=/etc/crateconfig
# Device name is the common part between MSATTN2 and MSATTN4
# to match any MSATTNx module 
DEVICE_NAME=MSATTN
DRIVER_NAME=msatt
DRIVER_VERSION=
CRATECONFIG=/etc/crateconfig
TRANSFER=/etc/transfer.ref
HOSTNAME=`hostname`

OUTPUT=":"
RUN=""
PACKET_DUMP=""

while getopts hvpnc:D:d:t:g: o
do	case $o in
	v)	OUTPUT="echo" ;;		# verbose
	n)	RUN=":" ;;			# dry run
	D)	DEVICE_NAME="$OPTARG" ;;
	d)	DRIVER_NAME="$OPTARG" ;;
	p)	PACKET_DUMP="dump_packet=1" ;;
	g)	DRIVER_VERSION=-"$OPTARG" ;;
	c)	CRATECONFIG="$OPTARG" ;;
	t)	TRANSFER="$OPTARG" ;;
	[h?])	echo >&2 "usage: $0 [-?hvn] [-D device_name] [-d driver_name] [-g driver_version] [-c crateconfig] [-t transfer]"
		exit ;;
	esac
done

$OUTPUT "Installing $DEVICE_NAME driver..."
INSMOD_ARGS=`awk -f transfer2insmod.awk $DEVICE_NAME $CRATECONFIG $TRANSFER`
if [ x"$INSMOD_ARGS" == x"" ] ; then
    echo "No $DEVICE_NAME declared in $TRANSFER, exiting"
    exit 1
fi

if [ x"$PACKET_DUMP" != x"" ] ; then
	INSMOD_ARGS="$INSMOD_ARGS $PACKET_DUMP"
fi

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

sh -c "$RUN rm -f /dev/msatt.*"
for minor in `awk -f luns.awk $DEVICE_NAME $TRANSFER` ; do
	sh -c "$RUN /bin/mknod -m 0666 /dev/msatt.$minor c ${MAJOR} $minor"
done
