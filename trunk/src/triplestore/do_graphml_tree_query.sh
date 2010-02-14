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

export NODE_QUERY=`./do_node_query_construct.sh "$2"`
export EDGE_QUERY=`./do_edge_query_construct.sh "$2"`


cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">

EOM



echo "<!-- NODE_QUERY: $NODE_QUERY -->"

echo "<!-- EDGE_QUERY: $EDGE_QUERY -->"

echo "<graph id=\"$2\" edgedefault=\"$4\">"
#Query to get the nodes.
./do_query.py "$1" "$NODE_QUERY" "<node id=\"%s\"/>" "$3"

./do_query.py "$1" "$EDGE_QUERY" "<edge id=\"%s\" source=\"%s\" target=\"%s\"/>" "$3"


echo "</graph>"

cat << EOM
</graphml>
EOM
