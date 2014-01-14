#!/bin/sh

OUTPUT=":"

$OUTPUT "Installing ipoctal driver"
(cat /proc/modules | grep -q "^ipack ") || /sbin/insmod ../ipack/ipack.ko

/sbin/insmod ipoctal.ko
