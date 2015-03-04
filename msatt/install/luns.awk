#
# transfer2insmod.awk - extract insmod parameters from transfer.ref
#
# usage: transfer2insmod.awk DEVICE_NAME [transfer_file]
#
# e.g.:
#  $ awk -f transfer2insmod.awk VD80 /acc/dsc/tst/cfv-864-cdv28/etc/transfer.ref
#
#  produces
#     insmod mil1553.ko bcs=1,2
#                       pci_buses=1,1
#                       pci_slots=13,14
#
# The bus number is not available from transfer.ref but by chance it is always 1
#

BEGIN {
	device_name = ARGV[1]
	delete ARGV[1]
}
# The match should work for two types of devices MSATTN2 & MSATTN4
# therefore the check is done with device-name=MSATTN
/^#\+#/ && (substr($6,1,6)) == device_name  && $4 == "CPC" {
	# decode transfer.ref line
	luns = luns " " $7
}

END {
	if (luns)
		print luns
}
