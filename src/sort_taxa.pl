#!/usr/bin/perl -w

#Script to put taxa in the taxa block and characters block 
#into the same order.
#Brandon Chisham
#5-5-2008

use strict;

sub isTaxLabelsLine {
    return /TAXLABELS/;
}


sub isMatrxLine {
    return /MATRIX$/;
}

sub sortTaxLabelsLine {
      
    my $tax_labels = shift;
    my @line       = split /[ \t]+/, $tax_labels;
    my $LABEL_MARK = "TAXLABELS";
    my $line_start = "";
    do {
       $line_start = shift @line;
    } until $line_start =~ /^$LABEL_MARK$/;
    #shift @line;
    my @sortedline  = sort @line;
    my $ret        = "\t" .$LABEL_MARK;
    foreach my $label (@sortedline){
        chomp( $label );
        $label =~ s/^(.*);$/$1/g;
	$ret .= " " . $label;
    }
    return $ret . ";\n";
}

my $taxLabelsSeen = 0;



while (<STDIN>){
    
    if (isTaxLabelsLine( $_  )){
	print sortTaxLabelsLine( $_ );
	$taxLabelsSeen = 1;
    }
    else {
	print $_;
    }
}
