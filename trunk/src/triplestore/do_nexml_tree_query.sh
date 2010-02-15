#!/bin/bash
#Brandon Chisham
#Feb. 15, 2010
#Script to generate NeXML format trees.

#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 directed/undirected

TU_QUERY=`do_tu_query_construct.sh "$2"`
NODE_QUERY=`do_node_query_construct.sh "$2"`
EDGE_QUERY=`do_edge_query_construct.sh "$2"`

cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
<nexml generator="CDAO Triplestore Query System" 
       version="0.8" 
       xmlns="http://www.nexml.org/1.0" 
       xmlns:nex="http://www.nexml.org/1.0" 
       xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns" 
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

echo "  <otus id=\"$3otus\">"
do_query.py "$1" "$TU_QUERY" "    <!-- %s --><otu id=\"%s\" />" "$3"
echo "  </otus>"

echo "  <trees id=\"$3trees\">"
echo "    <tree id=\"$2\">"
 do_query.py "$1" "$NODE_QUERY" "     <node id=\"%s\"/>" "$3"
 do_query.py "$1" "$EDGE_QUERY" "     <edge id=\"%s\" source=\"%s\" target=\"%s\"/>" "$3"
echo "    </tree>"
echo "   </trees>"
echo "</nexml>"
