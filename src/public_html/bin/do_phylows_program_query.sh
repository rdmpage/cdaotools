#!/bin/bash
# 
# File:   do_phylows_program_query.sh
# Author: bchisham
#
# Created on November 22, 2010, 11:20 AM
#

#Setup the environment
export SOFTWARE=$(echo "$REQUEST_URI" |grep -oE "program/[a-zA-Z]+" | sed "s/%20/ /g" | sed "s/+/ /g" | cut -f 2 -d "/")
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
#export STUDY_RESULT_TRANSFORM="../../cdao-store/study_result_to_html.xsl"
#export STUDY_AUTHORS_TRANSFORM="../../cdao-store/study_authors_to_html.xsl"
export TREE_URI="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree"

source ~/.bashrc
source ~/cdaostorecgi.rc


function open_document {
echo -n -e "ContentType: text/xml\n\n"
cat << EOM
<rdf:RDF
    xmlns:foaf="http://xmlns.com/foaf/0.1/" 
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:owl2="http://www.w3.org/2006/12/owl2#"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:cdao="http://www.evolutionaryontology.org/cdao/1.0/cdao.owl#"
    xmlns:owl="http://www.w3.org/2002/07/owl#">
EOM
}

function print_query {
	ALGO="$1"
cat << EOM
	PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
	PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
	PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
	SELECT ?tree 
	WHERE {
    		?study study:has_analysis ?analysis .
    		?analysis study:has_software '$ALGO' .
    		?analysis study:has_output_tree ?tree .
	}
EOM
}



function print_result {
   export SOFTWARE_QUERY=$(print_query "$SOFTWARE");
   echo "<!-- SOFTWARE_QUERY: $SOFTWARE_QUERY -->"
   TMP_QUERY_FILE=`mktemp`".rql";
   echo "$SOFTWARE_QUERY" > $TMP_QUERY_FILE;
    ROW_COUNT=0;
   for result in $(sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g" | sort);
   do
        echo "<cdao:Tree rdf:about=\"$TREE_URI/$result\"/>";
   done
   rm -f "$TMP_QUERY_FILE" 
}

function close_document {
  echo "</rdf:RDF>"
}

open_document;

print_result;

close_document;
