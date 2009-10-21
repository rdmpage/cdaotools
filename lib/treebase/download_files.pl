#!/usr/bin/perl -w
#Script to download nexus files from the treebase dump.

use strict;

my $base_url= $ARGV[0];
my $infile = $ARGV[1];
if ($base_url && $infile){
   open (INFILE, "<$infile") or die "Unable to open input: $!\n";
   while (<INFILE>){
       if (/\<A HREF=\"(.*)\"/){ system("wget $base_url$1" ); }
   }
}

