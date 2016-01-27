#
# svec-fd-luns.awk - create lun'ed devnodes from transfer.ref
#
# usage: svec-fd-luns.awk DEVICE_NAME [transfer_file]
#
# e.g.:
#  $ awk -f svec-fd-luns.awk SVECFDELAY \
#		/acc/dsc/tst/cfv-864-cdv28/etc/transfer.ref
# produces as output
#   FIXME: sh command

BEGIN {
	device_name = ARGV[1]
	delete ARGV[1]
}

/^#\+#/ && $6 ~ device_name {
	# decode transfer.ref line
	sl = $20
	ss = $21
	dev_id = sl * 64 + ss
	lun = $7
	printf("ln -sf %04x /dev/adc-100m14b.%d\n", dev_id, lun)
}
