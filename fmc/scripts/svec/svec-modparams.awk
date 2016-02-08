#
# svec-modparams.awk - extract insmod parameters from transfer.ref
#
# usage: svec-modparams.awk DEVICE_NAME [transfer_file]
#
# e.g.:
#  $ awk -f svec-modparams.awk FMC_SVEC /acc/dsc/tst/cfv-864-cdv28/etc/transfer.ref
#  $ awk -f svec-modparams.awk FMC-SVEC /acc/dsc/tst/cfv-864-cdv28/etc/transfer.ref
#
#  produces
#    vme_base=a0000000,vme_size=0x10000000,vme_am=0x39,vector=0x00,lun=0,slot=8
#

BEGIN {
	device_name = ARGV[1]
	delete ARGV[1]
	slot = ""
	a24_base_addr = ""
	vector = ""
	vme_size = ""
}

/^#\+#/ && $6 == device_name  && $4 == "VME" {
	# decode transfer.ref line
	lun = lun "," $7
	slot =  slot "," $20
	a24_base_addr = sprintf("%s,0x%s", a24_base_addr, $16)
	vector = vector "," $23
	level = level ",2"		# hack: always level 2
	fw_name = fw_name ",$GOLDEN"	# hack: always golden bitstream
	vme_am = vme_am ",0x39"		# hack: always A24
	vme_size = sprintf("%s,0x%s", vme_size, $17)

}

END {
	insmod_params = " "

	# take away the first comma in each vector of params
	if (lun)
		insmod_params = insmod_params " lun=" substr(lun, 2)
	if (slot)
		insmod_params = insmod_params " slot=" substr(slot, 2)
	if (a24_base_addr)
		insmod_params = insmod_params " vme_base=" substr(a24_base_addr, 2)
	if (vector)
		insmod_params = insmod_params " vector=" substr(vector, 2)
	if (fw_name)
		insmod_params = insmod_params " fw_name=" substr(fw_name, 2)
	if (vme_am)
		insmod_params = insmod_params " vme_am=" substr(vme_am, 2)
	if (vme_size)
		insmod_params = insmod_params " vme_size=" substr(vme_size, 2)

	print substr(insmod_params, 3)
}
