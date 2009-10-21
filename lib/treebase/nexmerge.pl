#!/usr/bin/perl -w
#Script to merge a matrix and it's correspoinding tree.
#Brandon Chisham
#September 29, 2009

my $matrix_file;
my $tree_file;


if (@ARGV >= 2){
   $matrix_file = $ARGV[ 1 ];
   $tree_file = $ARGV[ 2 ];
}

print `cat $matrix_file`;

my $tree_file_len = `wc -l | sed "s/^ *//g" | cut -f 1 -d` - 1;

print `tail -n $tree_file_len $tree_file`;

