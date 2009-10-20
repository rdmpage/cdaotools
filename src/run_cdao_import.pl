#!/usr/bin/perl -w
#Wrapper script for cdao translator
#Brandon Chisham
#5-7-2008


use strict;


my $input = "";
my $output = "";
my $isTempInput = 0;
my $INFLAG = "-i";
my $OUTFLAG = "-o";


$ENV{'PATH'} .= ":.";

for (my $i = 0; $i < @ARGV - 1; $i++){
     if ($ARGV[ $i ] =~ /^$INFLAG$/){ $input = $ARGV[ $i +1 ]; }
     elsif ($ARGV[ $i ] =~ /^$OUTFLAG$/){ $output = $ARGV[ $i + 1 ]; }
}

if ($input =~ /^$/){
    $input = "temp" . rand() . ".nex";
    $isTempInput = 1;
    open TMP_IN, ">$input" or die "Unable to open temp file $input $!\n";
    while (<STDIN>){
        print TMP_IN $_;       
    }
    close TMP_IN;
}

if ($output){
   system "cat $input | ./sort_taxa.pl | ./sort_matrix.pl | ./cdao-import -o $output";
}
else {
   system "cat $input | ./sort_taxa.pl | ./sort_matrix.pl | ./cdao-import ";
}
if ($isTempInput){
   unlink $input;
}

