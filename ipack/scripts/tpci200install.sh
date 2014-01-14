#!/bin/sh

(cat /proc/modules | grep -q "^ipack ") || /sbin/insmod ../ipack/ipack.ko
(cat /proc/modules | grep -q "^tpci200 ") || /sbin/insmod ../ipack/tpci200.ko

./make_dev_nodes
