#!/bin/bash
#Brandon Chisham
#Feb. 15, 2010
#Script to generate NeXML format trees.

#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 directed/undirected

export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"


TU_QUERY=`do_tu_query_construct.sh "$XMLNS$TREE_NAME"`
NODE_QUERY=`do_node_query_construct.sh "$XMLNS$TREE_NAME"`
EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`

cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
<nexml generator="CDAO Cdao Store Query System" 
       version="0.8" 
       xmlns="http://www.nexml.org/1.0" 
       xmlns:nex="http://www.nexml.org/1.0" 
       xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns"
       xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
       xmlns:data="$XMLNS" 
       xmlns:xml="http://www.w3.org/XML/1998/namespace" 
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
       xsi:schemaLocation="http://www.nexml.org/1.0 http://www.nexml.org/1.0/nexml.xsd">
       <!--
          Generated SqarQL Quer(y|ies). on `date`
       -->
       <!--
              $TU_QUERY
              $NODE_QUERY
              $EDGE_QUERY
       -->      
EOM

echo "  <otus id=\"'$XMLNS'otus\">"
do_query.py "$1" "$TU_QUERY" "    <!-- %s --><otu id=\"%s\" />" "$3"
echo "  </otus>"

echo "  <trees id=\"'$XMLNS$TREE_NAME'trees\">"
echo "    <tree id=\"$XMLNS$TREE_NAME\">"
 do_query.py "$GRAPH_CONFIG" "$XMLNS" "$NODE_QUERY" "     <node id=\"%s\" label=\"%s\"/>" "$EDGE_QUERY" "     <edge id=\"%s\" source=\"%s\" target=\"%s\"/>"
echo "    </tree>"
echo "   </trees>"
echo "</nexml>"
