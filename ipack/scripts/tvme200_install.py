#!	/usr/bin/env python

import sys
import re
import glob
import os
import argparse

transfer = "/etc/transfer.ref"
fieldnames = ( '# ln mln bus mtno module-type '
    ' lu W AM DPsz basaddr1 range1 W AM DPsz basaddr2 '
    ' range2 testoff sz sl ss L Vect' ).split()

def read_ioconfig(filename, module_type='IPOCTAL'):
    """get list of ioconfig parameters for a module type

    Returns a list of dictionaries, with keys the headers of ioconfig
    """
    res = []
    for l in file(filename):
        if not l.startswith('#+#'):
	    continue
	data = dict(zip(fieldnames, l.split()))
	if not data['module-type'] == module_type:
	    continue
	slot = int(data['sl'])
	res.append(data)
    return res

def generate_params(transfer):

    carriers = read_ioconfig(transfer, module_type='TVME200')
    ipoctals = read_ioconfig(transfer, module_type='IPOCTAL')

    lun = []
    base_address_ioid = []
    base_address_mem = []
    index = 0
    vectors = []
    symlinks = []

    for d in carriers:
        vector = 4 * ['-1']
        lun.append(d['lu'])
        base_address_ioid.append('0x' + d['basaddr1'])
        base_address_mem.append('0x' + d['basaddr2'])
        this_ipoctals = [ board for board in ipoctals
                                if board['sl'] == d['sl']]
        # Building symlinks and int vectors for ipoctals in current carrier
        # TODO: double-check against /sys/bus/ipack and report
        # inconsistencies
        for ip in this_ipoctals:
            for ch in xrange(8):
                symlinks.append(('/dev/ipoctal.%d.%s.%d' % ( index, ip['ss'], ch, ), 
                                 '/dev/ipoctal.%s.%d' % ( ip['lu'], ch, )))
            vector[int(ip['ss'])] = ip['Vect']
        index += 1
        vectors.append(','.join(vector))

    insmod_params = [
        'lun=' + ','.join(lun),
        'base_address_ioid=' +','.join(base_address_ioid),
        'base_address_mem=' + ','.join(base_address_mem),
        'irq=' + ','.join(vectors),
    ]

    return ' '.join(insmod_params), symlinks



if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--transfer', type=str,
                        help='transfer.ref file path', default=transfer)
    parser.add_argument('-n', '-v', '--dry-run', dest='dry_run',
                        action='store_true',
                        help='do nothing, just print actions')
    args = parser.parse_args()

    if args.dry_run:
        execute = lambda x : sys.stdout.write(x + '\n')
    else:
        execute = os.system

    insmod_params, symlinks  = generate_params(args.transfer)

    insmod = '/sbin/insmod tvme200.ko %s' % insmod_params
    execute(insmod)

    for src, dst in symlinks:
        execute('ln -s %s %s' % (src, dst))
