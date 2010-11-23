#!/bin/bash
#Script to list trees containing a given tu.

TU=`echo $REQUEST_URI | grep -oE "tu/[_a-zA-Z0-9%/]+" | sed "s/%20/_/g"  | sed 's/tu\///' | sed 's/\//\n/g'`

STATIC_WEB_DIR="../../"
TRANSFORM="tu_to_tree_list.xsl"
XSL="$STATIC_WEB_DIR/$TRANSFORM"
DATA="../tu/tu_to_filename.xml"

PHYLOWS_TREE_URL="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree"

source ~/cdaostorecgi.rc

function open_document {
    cat <<EOM
ContentType: text/xml

<?xml version="1.0"?>
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
    CURRENT=0;
   # echo "value of TU \"$TU\""
    for i in $TU; do
       CURRENT_SET=$(xsltproc --stringparam target "$i" "$XSL" "$DATA"  | sed 's/>/>\n/g' | grep "cdao:Tree" | grep -o -E 'Tree[0-9][0-9]*');
       TU_TREE_SET["$CURRENT"]=$(echo "$CURRENT_SET");       
       TU_NAME["$CURRENT"]="$i"
       ALL_TREES="$ALL_TREES $CURRENT_SET"
       CURRENT=$[$CURRENT + 1];
       CURRENT_SET=""
    done
    SET_SIZE="$CURRENT"
    CURRENT=0;
    
    ALL_TREES=$(echo "$ALL_TREES" | sort | uniq );    
    
    for tree in $ALL_TREES; 
    do
      DISCARD="false";
      for tu in $(seq 0 "$[$SET_SIZE - 1]");
      do
         # echo "<!-- current tree $tree current_tu: ${TU_NAME[$tu]} set is\" ${TU_TREE_SET[$tu]}\" -->"
         SET_SEARCH=$(echo ${TU_TREE_SET[$tu]} | grep -o -E "$tree" )
         echo "<!-- set search is: \"$SET_SEARCH\" -->"
         if [[ "$SET_SEARCH" == "" ]]; then
            echo "<!-- $tree was not in ${TU_NAME[$tu]}\'s set so it has been discarded from the results -->";
            DISCARD="true";
            break;
         fi
      done

      if [[ "$DISCARD" == "false" ]]; then
        RESULTS="$RESULTS $tree";
      fi
    done
    #RESULTS=$(echo "$RESULTS" | sort | uniq )
    for tree in $(echo $RESULTS | sort | uniq );
    do
       echo -e "\t<cdao:Tree rdf:about=\"$PHYLOWS_TREE_URL/$tree\" />"
    done
}

function close_document {
   echo "</rdf:RDF>"
}

open_document;

print_results;

close_document;
