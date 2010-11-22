#!/bin/bash
#Brandon Chisham
#March 21, 2010

#Script to implement phylows querying for tress in the triple store.

source ~/.bashrc

TREE_NAME=$( echo $REQUEST_URI | sed "s/\?$QUERY_STRING//"  | grep -oE "[-_a-zA-Z0-9]*$" )
FILE_NAME=$(grep "$TREE_NAME" tree_to_file.dat | cut -d' ' --fields=1)
BASE_URI="http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/tree"
TREE_URI="$BASE_URI/$TREE_NAME"

XMLNS="$TREE_URI#"
CDAONS="http://www.evolutionaryontology.org/cdao.owl#"


#NODE_QUERY="SELECT ?node ?node WHERE { ?node cdao:part_of <$TREE_URI>.  ?edge cdao:has_Parent_Node ?node.  }"
#EDGE_QUERY="SELECT ?edge ?src ?dest WHERE { ?src cdao:part_of <$TREE_URI>.  ?dest cdao:part_of <$TREE_URI>. ?edge cdao:has_Child_Node ?dest. ?edge cdao:has_Parent_Node ?src. }"

export NODE_QUERY=`do_node_query_construct.sh "$TREE_URI"`
export EDGE_QUERY=`do_edge_query_construct.sh "$TREE_URI"`


FORMAT=$(echo "$QUERY_STRING" | grep -oE "format=[a-zA-Z][a-zA-Z]*" | cut -d= --fields=2)

if [[ "$FORMAT" == "prolog" ]]; then
   echo -e -n "Content-type: text/plain\n\n"
   echo -e "% TREE: $TREE_NAME FORMAT: $FORMAT  "
   do_query.py "$TRIPLESTORE_CONFIG_STRING" "$XMLNS" "$NODE_QUERY" "node( '$TREE_NAME', '%s'). /*%s*/" "$EDGE_QUERY" "edge( '$TREE_NAME','directional', '%s', '%s', '%s')." | sed 's/\/\*.*\*\///g'
   cat ancestor_of.pl
else
   #echo -e -n "Content-type: text/xml\n\n"

   if [[ "$FORMAT" == "graphml" ]]; then
       cat << EOM
         <!DOCTYPE graphml:graphml [
           <!ENTITY cdao "$CDAONS">
           <!ENTITY data "$XMLNS">
           <!ENTITY tree "$TREE_NAME">
         ]>
        <graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
                 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                 xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
                 xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
                 http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
           <key id="d0" for="all" attr.name="IdLabel" attr.type="string"/>
           <!-- <key id="d1" for="edge" attr.name="color" attr.type="string">black</key>-->
           <!-- <key id="boostrap" for="edge" attr.name="weight" attr.type="double"/> -->
           <graph id="$XMLNS$TREEE_NAME" edgedefault="$TYPE">
	   <!-- NODE_QUERY: $NODE_QUERY -->
	   <!-- EDGE_QUERY: $EDGE_QUERY -->
          $(do_query.py "$TRIPLESTORE_CONFIG_STRING"  "$XMLNS" "$NODE_QUERY" "<node id=\"%s\"><data key=\"d0\">%s</data></node>" "$EDGE_QUERY" "<edge id=\"%s\" source=\"%s\" target=\"%s\"></edge>" | perl -p -e 's/<data key=\"d0\">http.*#/<data key=\"d0\">/g' )
             </graph>
	   </graphml>
EOM
   else
     #echo "$TREE_NAME"
      echo -e -n "Content-type: text/xml\n\n";
      echo "<rdf:RDF xmlns=\"http://www.evolutionaryontology.org/cdao.owl#\"
        xml:base=\"http://www.evolutionaryontology.org/cdao.owl\"
        xmlns:owl2xml=\"http://www.w3.org/2006/12/owl2-xml#\"
        xmlns:cdao=\"http://www.evolutionaryontology.org/cdao/1.0/cdao.owl#\"
        xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"
        xmlns:dc=\"http://purl.org/dc/elements/1.1/\"
        xmlns:owl2=\"http://www.w3.org/2006/12/owl2#\"
        xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"
        xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"
        xmlns:owl=\"http://www.w3.org/2002/07/owl#\">"

      echo -e "<!-- NODE: $NODE_QUERY \n EDGE: $EDGE_QUERY \n REQUEST_URI: $REQUEST_URI \n  -->"
      echo "<owl:Ontology rdf:about=\"\"></owl:Ontology>"
      echo "<cdao:Tree rdf:about=\"$TREE_URI\"/>"
      echo "  <!-- <rdf:type rdf:about=\"$CDAONS""Tree\" /> -->"
      do_query.py "$TRIPLESTORE_CONFIG_STRING" "$TREE_URI"  "$NODE_QUERY" "
      <cdao:Node rdf:about=\"%s\">
        <cdao:belongs_to_Tree rdf:resource=\"$TREE_URI\"/>
      </cdao:Node>
         <!-- %s -->
      " "$EDGE_QUERY" "
      <cdao:DirectedEdge rdf:about=\"%s\">
         <cdao:belongs_to_Tree rdf:resource=\"$TREE_URI\"/>
         <cdao:has_Parent_Node rdf:resource=\"%s\"/>
         <cdao:has_Child_Node rdf:resource=\"%s\"/>
      </cdao:DirectedEdge>" 
     echo "</rdf:RDF>"
  fi
fi
