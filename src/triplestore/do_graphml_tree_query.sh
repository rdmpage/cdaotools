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

export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"



export NODE_QUERY=`do_node_query_construct.sh "$XMLNS$TREE_NAME"`
export EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`


cat << EOM
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE graphml:graphml [
  <!ENTITY cdao "$CDAONS">
  <!ENTITY data "$XMLNS">
  <!ENTITY tree "$TREE_NAME">
]>


<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:data="$XMLNS"
    xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">

EOM



echo "<!-- NODE_QUERY: $NODE_QUERY -->"

echo "<!-- EDGE_QUERY: $EDGE_QUERY -->"
echo "<key id=\"name\" for=\"node\" attr.name=\"IdLabel\" attr.type=\"string\"/>"
echo "<key id=\"boostrap\" for=\"edge\" attr.name=\"weight\" attr.type=\"double\"/>"

echo "<graph id=\"data:$TREEE_NAME\" edgedefault=\"$TYPE\">"

#Query to get the nodes.
do_query.py "$GRAPH_CONFIG"  "$XMLNS" "$NODE_QUERY" "<node id=\"%s\"><data key=\"name\">%s</data></node>" "$EDGE_QUERY" "<edge id=\"%s\" source=\"%s\" target=\"%s\"><data key=\"d1\"></data></edge>" | perl -p -n -e 's/$ENV{XMLNS}/&data;/g' | sed 's/<data key="name">&data;\(.*\)<\/data>/<data key="name">\1<\/data>/g'

echo "</graph>"

cat << EOM
</graphml>
EOM
