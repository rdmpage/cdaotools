#!/usr/bin/perl -w

#Script to sort taxa in characters block
#Brandon Chisham
#5-5-2008

use strict;
my $END_MARK = "END";

sub isMatrixLine {
    return /MATRIX$/;
}

sub isEndLine {
    return /$END_MARK;$/
}


sub printPreMatrix { 
    while ( <STDIN> ){
	if ( isMatrixLine( $_ )){
	    print $_;
	    return;
	}
	print $_;
    }
}

sub getMatrixLines {
    my $ret = "";
    while ( <STDIN> ){
	if (isEndLine($_ )){ return $ret; }
        $ret .= $_;
    }
    return $ret;
}

sub printSortedMatrixLines {  
    
    my $lines = shift;

    my @TAX_LINES = split /\n/, $lines;
    my @SORTED_TAX = sort @TAX_LINES;
    foreach my $tax_line (@SORTED_TAX){
	if ($tax_line =~ /;$/){}
	else {
	    print $tax_line . "\n";
        }
    }
    print "\t;\n" . $END_MARK . ";\n";
}

sub printRemainder {
    while (<STDIN>){ print $_; }
}

printPreMatrix();
printSortedMatrixLines( getMatrixLines() );
printRemainder();
