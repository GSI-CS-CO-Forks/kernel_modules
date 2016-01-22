#!/usr/bin/perl
# Install sis33 modules from a transfer.ref file
#
# first the transfer.ref file is parsed and then, after some sanity checks,
# insmod is called with the parameters extracted from transfer.ref.
#
# call with 'script.pl --type 00' to install sis3300 modules only.
# ditto for SIS33'20' modules.
# by default it tries to install all the modules it finds.

use strict;
use warnings;
use Getopt::Long;

# module types to be installed
my @types;

# supported module types
my $supported = "(0|2)0";

GetOptions("type=s" => \@types) or die("Failed to parse arguments\n");

# allow comma-separated input
@types = split(/,/, join(',', @types));

# sanity check the input
for my $type (@types) {
    die("Invalid type $type (valid: $supported\n") if $type !~ m/^$supported$/;
}

# install all modules by default
if (!@types) {
    @types = qw/00 20/;
}

my $path = '/etc/transfer.ref';
my @AoH;
my @keys = ('ln', 'mln', 'bus', 'mtno', 'module-type', 'lu', 'W1', 'AM1',
	    'DPsz1', 'basaddr1', 'range1', 'W2', 'AM2', 'DPsz2', 'basaddr2',
	    'range2', 'testoff', 'sz', 'sl', 'ss');

my %base_addrs;

open(INPUT, "<$path") or die ("$path not found");
# put all the described modules in an array of hashes (AoH)
LINE: while (<INPUT>) {
    next LINE if $_ !~ m|^\#\+\#|;

    chomp;
    my @values = split(/\s+/, $_);
    # remove the first '#+#'
    shift @values;

    my $href;
    foreach (@keys) {
	$href->{$_} = shift @values;
    }

# only add SIS33 modules to the AoH
    next LINE if $href->{'module-type'} !~ m/^SIS33$supported$/;

# don't add modules with index values that we have already picked up
    if ($base_addrs{"$href->{'lu'}"}) {
	print "warning: $href->{'module-type'}.$href->{'lu'} has an already taken index number and won't be installed\n";
    } else {
	push @AoH, $href;
	$base_addrs{$href->{'lu'}} = $href->{'basaddr1'};
    }
}
close(INPUT);

# If we found at least a module, then sis33.ko should be installed
# But if the sis33 module is already there, we won't install it again
if (@AoH) {
    if (!module_is_loaded('sis33')) {
	system('insmod sis33.ko') == 0 or die("'insmod sis33.ko' failed");
    }
} else {
    die "No SIS33$supported modules found in $path.\n";
}

# install the modules in @types, catching repeated entries.
my %installed;
for my $module_type (@types) {
    if (!defined $installed{"$module_type"}) {
	sis33_install($module_type, \@AoH);
	$installed{"$module_type"} = 1;
    } else {
	print "warning: repeated type $module_type\n";
    }
}

sub module_is_loaded {
    my $module = shift;

    open(LSMOD, '/proc/modules') or die('Cannot open /proc/modules.\n');
    while (<LSMOD>) {
	chomp;
	my ($modname, $rest) = split(/\s+/, $_);

	if ($modname =~ m/^$module$/) {
	    close(LSMOD);
	    return 1;
	}
    }
    close(LSMOD);
    return 0;
}

sub sis33_install {
    my $type = shift;
    my $AoHref = shift;
    my $index_parm;
    my $base_parm;

  ENTRY: foreach my $href (@{$AoHref}) {
      next ENTRY if $href->{'module-type'} !~ m/SIS33$type/;

      if (defined $base_parm) {
	  $base_parm = $base_parm.',0x'.$href->{'basaddr1'};
	  $index_parm = $index_parm.','.$href->{'lu'};
      } else {
	  $base_parm = '0x'.$href->{'basaddr1'};
	  $index_parm = $href->{'lu'};
      }
  }
    if (defined $base_parm) {
	my $insmod = "insmod sis33$type.ko base=$base_parm index=$index_parm";
	system($insmod) == 0 or die("$insmod failed");
    }
}
