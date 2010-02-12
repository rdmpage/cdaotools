#!/usr/bin/perl -w

use strict;

my $SEARCH_PROG = "./search_tree_file";
my $TREE_FILE = "./tree_to_file.dat";
my $BASE_URI = "http://www.cs.nmsu.edu/~epontell";

my %formats = ( g => "graphml", 
                graphml => "graphml", 
                p => "prolog", 
                prolog => "prolog" );
my %suffix  = ( graphml => ".gml", prolog => ".pl" );

my $oformat ="";

print "Enter output format (g/p): ";
$oformat = <STDIN>;
chomp( $oformat );


$oformat = $formats{ $oformat };

my $tmpname = "/tmp/TMP_CDAO_QUERY_INTERACTIVE".rand();

system "$SEARCH_PROG  $TREE_FILE $tmpname";

my $trees_to_process=`cat $tmpname`;
unlink $tmpname;

#my $trees_to_process=`$SEARCH_PROG $TREE_FILE`;

my @tree_lines = split /\n/, $trees_to_process;

foreach my $line ( @tree_lines ){
   my @split_line = split / /, $line;
   my $xml_base = $BASE_URI . "/" . $split_line[0];
   my $tree_name = $xml_base . "#" .$split_line[1];
   my $out_name = $split_line[1] . $suffix{$oformat};
   my $command = "./do_" . $oformat . "_tree_query.sh  \"$ARGV[0]\" \"$tree_name\" \"$xml_base\" > $out_name";
   print "running query: $command\n";
   system( $command );
   print "Saving results for tree: $tree_name in: $out_name\n";
} 
