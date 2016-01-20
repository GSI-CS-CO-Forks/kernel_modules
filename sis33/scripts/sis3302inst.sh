#!/bin/sh

vmedesc $* --format=flat SIS3302 | ./sis33inst.pl sis3320
