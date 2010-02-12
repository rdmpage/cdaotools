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

export NODE_QUERY="SELECT ?node WHERE { ?node <$cdao#part_of> <$2>.  ?node rdf:type <$cdao#Node> . }"
export EDGE_QUERY="SELECT ?edge ?src ?dest WHERE { ?dest <$cdao#part_of> <$2>. ?src <$cdao#part_of> <$2>. ?edge <$cdao#has_Child_Node> ?dest . ?edge <$cdao#has_Parent_Node> ?src. }"

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
./do_query.py "$1" "$NODE_QUERY" "<node id=\"%s\"/>\\n" "$3"

./do_query.py "$1" "$EDGE_QUERY" "<edge id=\"%s\" source=\"%s\" target=\"%s\"/>" "$3"


echo "</graph>"

cat << EOM
</graphml>
EOM
