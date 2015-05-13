#!/bin/sh

# equate SIS3320 and SIS3320-250 modules in transfer.ref
TEMPFILE=$(mktemp)
sed '/#+#/s!SIS3320-250!SIS3320    !' /etc/transfer.ref > $TEMPFILE

vmedesc $* --format=flat SIS3320 --transfer=$TEMPFILE | ./sis33inst.pl sis3320

rm $TEMPFILE
