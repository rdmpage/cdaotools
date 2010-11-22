#!/bin/bash
# 
# File:   do_phylows_study_query.sh
# Author: bchisham
#
# Created on Novemember 21, 2010, 12:05 PM
#

#Setup the environment
export STUDY=`echo $REQUEST_URI |grep -oE "study[/]+[S][0-9]+" | sed "s/%20/ /g"  | cut -f 2 -d "/"`
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
export STUDY_RESULT_TRANSFORM="../../study_result_to_foaf.xsl"
export STUDY_AUTHORS_TRANSFORM="../../study_authors_to_foaf.xsl"
export AUTHOR_URI="../author/html?last="
source ~/cdaostorecgi.rc

export STUDY_QUERY=$( do_study_detail_query_construct.sh "$STUDY" )
export AUTHOR_QUERY=$( do_study_list_authors.sh "$STUDY" )
export TREE_QUERY=$( do_study_list_trees.sh "$STUDY" )

function open_document {
   echo -n -e "ContentType: text/xml\n\n";
   cat << EOM
   <rdf:RDF
    xmlns:foaf="http://xmlns.com/foaf/0.1/" 
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:owl2="http://www.w3.org/2006/12/owl2#"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:owl="http://www.w3.org/2002/07/owl#"
    xmlns:cdao="http://www.evolutionaryontology.org/cdao/1.0/cdao.owl#">
EOM
}

function print_results {
    echo "<!-- STUDY_QUERY: $STUDY_QUERY -->"
    echo "<!-- AUTHOR_QUERY: $AUTHOR_QUERY -->"
    echo "<!-- TREE_QUERY: $TREE_QUERY -->"

    TMP_QUERY_FILE=`mktemp`".rql";
    #TMP_RESULTS_FILE=`mktemp`".xml";
    TMP_AUTHOR_QUERY_FILE=`mktemp`".rql";
    TMP_TREE_QUERY_FILE=`mktemp`".rql"
    echo "$STUDY_QUERY" > $TMP_QUERY_FILE;
    echo "$AUTHOR_QUERY" > $TMP_AUTHOR_QUERY_FILE;
    echo "$TREE_QUERY" > $TMP_TREE_QUERY_FILE;
    echo "<foaf:Document  rdf:about=\"http://www.cs.nmsu.edu$REQUEST_URI\">"
    AUTHORS=$(sparql -q --results xml --query "$TMP_AUTHOR_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_AUTHORS_TRANSFORM - | sed 's/ /_/g' | sed 's/_rdf/ rdf/g' | sed 's/_xmlns/ xmlns/g' | sed 's/^_*//g' | sed 's/\/>/\/>\n/g'  )
    echo "$AUTHORS"
    sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_RESULT_TRANSFORM -

   echo "</foaf:Document>"
    for result in $(sparql -q --results xml --query "$TMP_TREE_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g");
   do
        echo "<cdao:Tree rdf:about=\"http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree/$result\">";
        echo "$AUTHORS"
        echo "</cdao:Tree>"
   done
   echo "$AUTHORS" | sed 's/maker/Person/g' | sed 's/resource/about/g'
   rm -f "$TMP_AUTHOR_QUERY_FILE" "$TMP_QUERY_FILE" "$TMP_TREE_QUERY_FILE" "$TMP_RESULTS_FILE"

}

function close_document {
    echo "</rdf:RDF>"
}

open_document;

print_results;

close_document;
