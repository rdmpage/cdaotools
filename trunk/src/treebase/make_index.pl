#!/usr/bin/perl -w
#Brandon Chisham
#Script to create index file.

use strict;

my %tus_to_files;
my %file_to_trees;
my $EXTRACTOR = "extract_trees.pl";
my $xmlns = "http://www.cs.nmsu.edu/~epontell";

foreach my $file (@ARGV){
   my $contents = `$EXTRACTOR $file`;
   my @lines = split /\n/, $contents;
   foreach my $line (@lines){
       if ($line =~ /^TU ([-a-zA-Z0-9_]+)$/){
           $tus_to_files{ $1 } .= $file . ",";
       }
       if ($line =~ /^TREE ([-a-zA-Z0-9_]+) .*$/){
           $file_to_trees{ $file } .= $1 . ",";
       }
   }
}

print "<?xml version=\"1.0\"?>\n";
print "<tu-mappings>\n";
foreach my $tu (keys %tus_to_files){
  my $file_list = $tus_to_files{ $tu };
  $file_list =~ s/,$//;
  print "\t<tu id=\"$tu\">\n";
#print $tu . "," . $file_list . "\n";
  my @tu_files = split /,/, $file_list;
  foreach my $tu_file (@tu_files){
     my $tree_names =  $file_to_trees{ $tu_file };
     $tree_names =~ s/,$//;
     my @sptree_names = split /,/, $tree_names;
     @sptree_names = sort @sptree_names;
     foreach my $tree_name (@sptree_names){
        print "\t\t<tree name=\"$tree_name\"/>\n";
     }

  }
  print "\t</tu>\n"
}
print "</tu-mappings>";
