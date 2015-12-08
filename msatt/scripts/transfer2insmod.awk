	#
# transfer2insmod.awk - extract insmod parameters from transfer.ref
#
# usage: transfer2insmod.awk DEVICE_NAME [transfer_file]
#
# e.g.:
#  $ awk -f transfer2insmod.awk <DEVICE_NAME> transfer.ref
#
#  produces
#     insmod msatt.ko lun=1,2
#                       bus=1,1
#                       slot=13,14
#
# The bus number is not available from transfer.ref but by chance it is always 1
#

BEGIN {
	device_name = ARGV[1]
	delete ARGV[1]
	crateconfig = ARGV[2]
	delete ARGV[2]
	while (getline <crateconfig > 0) {
		slot_to_pci_bus[$1] = $2
		slot_to_pci_slot[$1] = "0x" $3
	}
	luns = ""
	slots = ""
}

# The match should work for two types of devices MSATTN2 & MSATTN4
# therefore the check is done with device-name=MSATTN
/^#\+#/ && (substr($6,1,6)) == device_name  && $4 == "CPC" {
	# decode transfer.ref line
	luns = luns "," $7
	slots =  slots "," $20
	pci_bus  = pci_bus "," slot_to_pci_bus[$20]
	pci_slot = pci_slot "," slot_to_pci_slot[$20]
}

END {
	insmod_params = " "

	if (luns)
		insmod_params = insmod_params "lun=" substr(luns, 2)
	if (pci_bus) {
		insmod_params = insmod_params " bus=" substr(pci_bus, 2)
		insmod_params = insmod_params " slot=" substr(pci_slot, 2)
	}
	print insmod_params
}
