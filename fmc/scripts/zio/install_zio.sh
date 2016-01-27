#! /bin/sh

DRIVER_VERSION=
DRIVER_NAME=zio
DRIVER_PATH=$DRIVER_NAME.ko
INSMOD_ARGS=
OUTPUT=":"
RUN=""

while getopts hvnd: o
do	case $o in
	v)	OUTPUT="echo" ;;		# verbose
	n)	RUN=":" OUTPUT="echo" ;;	# dry run
	d)	DRIVER_NAME="$OPTARG" ;;
	[h?])	echo >&2 "usage: $0 [-?hvn] [-d driver]"
		echo >&2 "	-h, -?		: help"
		echo >&2 "	-v		: verbose"
		echo >&2 "	-d driver	: driver name (e.g. zio)"
		exit ;;
	esac
done

# manage exception
if [ x"$DRIVER_VERSION" = x"" -a -r $HOSTNAME ] ; then
	DRIVER_VERSION=`cat $HOSTNAME`
	echo "installing special version $DRIVER_VERSION for $HOSTNAME"
	DRIVER_VERSION=-$DRIVER_VERSION
fi

INSMOD_CMD="/sbin/insmod $DRIVER_PATH$DRIVER_VERSION $INSMOD_ARGS"
$OUTPUT installing $DRIVER_NAME by $INSMOD_CMD
sh -c "$RUN $INSMOD_CMD"
