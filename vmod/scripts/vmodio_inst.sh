#!/bin/sh

DEVICE_NAME=VMODIO
DRIVER_NAME=vmodio
DRIVER_VERSION=
TRANSFER=/etc/transfer.ref

OUTPUT=":"
RUN=""

echo "Installing modulbus_register driver..."
(cat /proc/modules | grep -q '^modulbus_register ' ) || /sbin/insmod modulbus_register.ko

$OUTPUT "Installing $DEVICE_NAME driver..."
INSMOD_ARGS=`awk -f $DRIVER_NAME.awk $DEVICE_NAME $CRATECONFIG $TRANSFER`
if [ x"$INSMOD_ARGS" == x"" ] ; then
    echo "No $DEVICE_NAME declared in $TRANSFER, exiting"
    exit 1
fi

# manage exception
if [ x"$DRIVER_VERSION" = x"" -a -r $HOSTNAME ] ; then
	DRIVER_VERSION=`cat $HOSTNAME`
	echo "$DRIVER_NAME installing special version $DRIVER_VERSION for $HOSTNAME"
	DRIVER_VERSION=-$DRIVER_VERSION
fi

INSMOD_CMD="/sbin/insmod $DRIVER_NAME.ko$DRIVER_VERSION $INSMOD_ARGS"
$OUTPUT "$DRIVER_NAME install by [$INSMOD_CMD]"
sh -c "$RUN $INSMOD_CMD"
