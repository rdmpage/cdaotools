#!/bin/bash

#PhyloWS author lookup

source ~/.bashrc

export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$TRIPLESTORE_CONFIG_STRING"
export GRAPH_FILE="http://www.cs.nmsu.edu/~$(whoami)/dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
export STUDY_URI="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/study/"
export FIRST_NAME=`echo $REQUEST_URI |grep -oE "first[/]+[-'%a-zA-Z0-9]+" | sed "s/%20/ /g"  | cut -f 2 -d "/"`
export LAST_NAME=`echo $REQUEST_URI | grep -oE "last[/]+[-'%a-zA-Z0-9]+" | sed "s/%20/ /g"  | cut -f 2 -d "/"`
export AUTHOR_QUERY=$(do_author_work_list_query_construct.sh "$LAST_NAME" "$FIRST_NAME");
export XMLNS="http://www.cs.nmsu.edu/~bchisham/study.owl#"
export PHYLOWS_BASE="http://www.cs.nmsu.edu"

export STUDY_URL="$PHYLOWS_BASE/~$(whoami)/cgi-bin/phylows/study"
source ~/cdaostorecgi.rc

function open_document {
    cat <<EOM
ContentType: application/xml

<?xml version="1.0"?>
<rdf:RDF
    xmlns:foaf="http://xmlns.com/foaf/0.1/" 
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:owl2="http://www.w3.org/2006/12/owl2#"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:owl="http://www.w3.org/2002/07/owl#">
EOM
}

#Print the results for the author.
function print_result {
   echo "<!-- AUTHOR_QUERY: $AUTHOR_QUERY -->"
   echo "<foaf:Person rdf:about=\"$PHYLOWS_BASE$REQUEST_URI\">
            <foaf:givenName>$FIRST_NAME<foaf:givenName>
            <foaf:familyName>$LAST_NAME</foaf:familyName>"
      TMP_QUERY_FILE=`mktemp`".rql";
   echo "$AUTHOR_QUERY" > $TMP_QUERY_FILE;
   RESULT_SIZE=0;
   for result in $(sparql -q --results text --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE"  | grep -oE "<.*>" | sed "s/<$ESCAPED_DUMP_URI\(.*\)>/\1/g" );
   do
        RESULT_SIZE=$[ $RESULT_SIZE + 1 ]
        echo "<foaf:Publication rdf:about=\"$STUDY_URL/$result\" />";
   done
   echo "</foaf:Person>"
   rm -f $TMP_QUERY_FILE
}


function close_document {
echo "</rdf:RDF>"
}

open_document;

print_result;

close_document;
