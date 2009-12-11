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
open TREEIN, "<$infile" or die "Unable to open input file:$!\n";

while (<TREEIN>){

  if (/TITLE ([a-zA-Z0-9]+);/){
      $title = $1;
#print "TITLE: $title\n";
  }
  elsif (/\t(\d+)\t([a-zA-Z0-9_]+)/){
      $taxon_ids{ $1 } = $2;
#print "taxon id: $1 taxon name: $2\n";
  }
  elsif (/TREE\W+([-a-zA-Z0-9_]+)\W+=\W+(\[&[R|U]\])\W+(\(.*\));$/){
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

#print "<?xml version=\"1.0\"?>\n";
#print "<rdf:RDF\n";
#print "\txmlns=\"http://www.cs.nmsu.edu/~epontell/$infile\"\n";
#print "\txml:base=\"http://www.cs.nmsu.edu/~epontell/$infile\"\n";
#print "\txmlns:owl11=\"http://www.w3.org/2006/12/owl11#\"\n";
#print "\txmlns:owl11xml=\"http://www.w3.org/2006/12/owl11-xml#\"\n";
#print "\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n";
#print "\txmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n";
#print "\txmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n";
#print "\txmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n";
#print "\txmlns:cdao=\"http://www.evolutonaryontology.org/cdao.owl#\"\n";
#print "\txmlns:amino-acid=\"http://www.co-ode.org/ontologies/amino-acid/2005/10/11/amino-acid.owl#\">\n";
#print "<owl:Ontology rdf:about=\"\">\n";
#print "\t<owl:imports rdf:resource=\"http://www.evolutionaryontology.org/cdao.owl\"/>\n";
#print "</owl:Ontology>\n";

#foreach my $key ( keys %taxon_ids ){
#    print "<cdao:TU rdf:ID=\"$taxon_ids{ $key }\"/>\n";
#}

print "#NEXUS\n";
print "BEGIN TREES;\n";
foreach my $tree ( keys %trees ){
    print "TREE $tree $isRooted{ $tree } $trees{ $tree };\n";
}
print "END;";
