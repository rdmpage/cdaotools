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

export SPARQL_QUERY="SELECT ?node WHERE { ?node $cdao#beongs_to_Tree $2.  ?node rdf:type $cdao#Node . }"

cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">

EOM

echo "<graph id=\"$2\"" edgedefault=\"$4\">
#Query to get the nodes.
./do_query.py $1 $SPARQL_QUERY "<node id=\"%s\"/>\\n" $3


echo "</graph>"

cat << EOM

</graphml>

EOM
