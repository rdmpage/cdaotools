#!/bin/bash

#Script to dump nodes in GraphML Format
#Brandon Chisham
#Feb. 11, 2010
#Description:
#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 directed/undirected 
#Note do_query.py expects DBCONFIG_STRING QUERY_STRING FORMAT_STRING XML_BASE_URI
export TREE_DAT_FILE=~bchisham/tree_to_file.dat
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"

if [[ "$TFILE" == "" ]]; then
        export TFILE=$(grep "$TREE_NAME" "$TREE_DAT_FILE" | cut -d' ' --fields=1);
fi
if [[ "$TREETYPE" == "" ]]; then
       export TREETYPE=$( grep "$TREE_NAME" "$TREE_DAT_FILE" | cut -d' ' --fields=3 ); 
fi


export NODE_QUERY=`do_node_query_construct.sh "$XMLNS$TREE_NAME"`
export EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`


cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml"
      xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
      xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
      xmlns:xsd="http://www.w3.org/2001/XMLSchema#"
      xmlns:cc="http://creativecommons.org/ns#"
      xmlns:dc="http://purl.org/dc/elements/1.1/"
      xmlns:foaf="http://xmlns.com/foaf/0.1/"
      xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
      xml:lang="en">
 <head profile="http://www.w3.org/1999/xhtml/vocab">
 <title>Cdao Store Query System</title>
      <link rel="stylesheet" type="text/css" href="../../style.css"/>
      <link rel="alternate" title="Project News" href="http://sourceforge.net/export/rss2_keepsake.php?group_id=282379" type="application/rss+xml"/>
      <link rel="SHORTCUT ICON" href="../cdao-store-favicon.ico" />
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 </head>
 <body about="http://www.evolutionaryontology.org/cdao.owl">

EOM



echo "<!-- NODE_QUERY: $NODE_QUERY -->"

echo "<!-- EDGE_QUERY: $EDGE_QUERY -->"
#echo "<key id=\"name\" for=\"node\" attr.name=\"IdLabel\" attr.type=\"string\"/>"
#echo "<key id=\"boostrap\" for=\"edge\" attr.name=\"weight\" attr.type=\"double\"/>"
echo "<div class=\"main-content\" style=\"scroll: auto;\">"
echo "<h1>Cdao Store Query System</h1>"
echo "<p><a href="../../cdao-store/index.html"><img src=\"../../cdao-triplestore-logo.jpg\" alt=\"Cdao-Store Logo\" style=\"border: 0px;\" /></a></p>"
echo "<table><form action=\"../nca/prolog\" metod=\"get\">"
echo "<input type="hidden" id=\"tree\" name=\"tree\" value=\"$TREE_NAME\">"
echo "<input type=\"hidden\" id=\"file\" name=\"file\" value=\"$TFILE\"/>"
echo "<input type=\"hidden\" id=\"type\" name=\"type\" value=\"$TREETYPE\"/>"
#Query to get the nodes.
do_query.py "$GRAPH_CONFIG"  "$XMLNS" "$NODE_QUERY" "<tr><td>%s</td><td><input type=\"checkbox\" id=\"node\" name=\"node\" value=\"%s\"></td></td>";
echo "<tr><td colspan=\"2\"><input class=\"button\"type=\"submit\" value=\"Nearest Common Ancestor\"/><input class=\"button\" type=\"reset\" value=\"Reset\"/><td></tr>"
echo "</form></table>"
echo "</div>"

cat << EOM
</body>
</html>
EOM
