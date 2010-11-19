#!/usr/bin/perl -w

use strict;

my @query_times;

my $current_query = 0;


foreach my $arg ( @ARGV ){
    $current_query = 0;
    open IN, "<$arg" or die "unable to open $!\n";
    while ( <IN> ){
	if (/^real\t0m([0-9]+\.[0-9][0-9][0-9])s/){
	   print STDERR "$_ ";
	   @query_times[ $current_query++ ] += $1;
	   print STDERR "current_query: $current_query current_host: $arg time: $1\n";
	}
    }
    close IN;
}

$current_query= 0;
foreach my $timing ( @query_times ){

    print "$current_query ($query_times[$current_query] / $#ARGV ) is " . $query_times[$current_query] / $#ARGV . "\n";
    $current_query++;
}


