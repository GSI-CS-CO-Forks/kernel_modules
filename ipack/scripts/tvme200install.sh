#!/bin/sh

OUTPUT=":"

$OUTPUT "Installing ipack driver"
(cat /proc/modules | grep -q "^ipack ") || /sbin/insmod ipack.ko

./tvme200_install.py
