#!/usr/bin/perl -w
#Brandon Chisham
#Dec. 10, 2009
#Script to extract translated trees from a nexus trees file.

use strict;

my %taxon_ids;
my %trees;
my %isRooted;

my $title;
my $tree_title;
my $tree_description;
my $infile = $ARGV[0];
open TREEIN, "<$infile" or die "Unable to open input file ($infile): $!\n";

while (<TREEIN>){

  if (/TITLE ([a-zA-Z0-9]+);/){
      $title = $1;
#print "TITLE: $title\n";
  }
  elsif (/\t(\d+)\t([a-zA-Z0-9_]+)/){
      $taxon_ids{ $1 } = $2;
#print "taxon id: $1 taxon name: $2\n";
  }
  elsif (/TREE\W+([-a-zA-Z0-9_]+)\W+=\W+(\[&[R|U]\])\W+(\(.*\));/){
    $tree_title = $1;
    $isRooted{ $tree_title }   = $2;
    $tree_description = $3;
     foreach my $key (keys %taxon_ids){
#print "\"$key\"\n";
       $tree_description =~ s/,$key,/,$taxon_ids{$key},/;
       $tree_description =~ s/\($key,/\($taxon_ids{$key},/;
       $tree_description =~ s/,$key\)/,$taxon_ids{$key}\)/;
            }
     $trees{ $tree_title } = $tree_description;
#print "Tree title: $tree_title description: $tree_description\n";
  }
}

close TREEIN;
#Print the list of TU's
foreach my $key ( keys %taxon_ids ){
    print "TU $taxon_ids{ $key }\n";
}
#Print the list of Trees.
foreach my $tree ( keys %trees ){
    print "TREE $tree $isRooted{ $tree } $trees{ $tree };\n";
}
#print "END;";
