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

source ~/cdaostorecgi.rc

PHYLOWS_TREE_URI="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree"

export NODE_QUERY=`do_node_query_construct.sh "$XMLNS$TREE_NAME"`
export EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`


function print_results {
echo "<!-- NODE_QUERY: $NODE_QUERY -->"

echo "<!-- EDGE_QUERY: $EDGE_QUERY -->"
#echo "<key id=\"name\" for=\"node\" attr.name=\"IdLabel\" attr.type=\"string\"/>"
#echo "<key id=\"boostrap\" for=\"edge\" attr.name=\"weight\" attr.type=\"double\"/>"
#echo "<div class=\"wrap-shadow\"><div class=\"wrap\"><div class=\"header\">"
#echo "<h1 class=\"header\"><a class=\"header\" href="../../index.php"><img src=\"../../cdao-triplestore-logo.jpg\" alt=\"Cdao-Store Logo\" style=\"border: 0px;\" /></a>Cdao Store</h1></div>"

#echo "<div id=\"content\" class=\"content\" style=\"scroll: auto;\">"
echo "<form action=\"../node/html\" method=\"get\"><table>"
echo "<tr><td colspan=\"2\">
           Nearest Common Ancestor: <input type=\"radio\" name=\"qtype\" checked=\"checked\" value=\"nca\" /> Minimum Spanning Clade <input type=\"radio\" name=\"qtype\" value=\"msc\"/>
       <input type=\"hidden\" id=\"tree\" name=\"tree\" value=\"$TREE_NAME\"/>
       </td></tr>"
echo ""
#echo "<input type=\"hidden\" id=\"file\" name=\"file\" value=\"$TFILE\"/>"
#echo "<input type=\"hidden\" id=\"type\" name=\"type\" value=\"$TREETYPE\"/>"
#Query to get the nodes.
#do_query.py "$GRAPH_CONFIG"  "$XMLNS" "$NODE_QUERY" "<tr><td>%s</td><td><input type=\"checkbox\" id=\"node\" name=\"node\" value=\"%s\"></td></td>" | sed 's/http:[-_~./a-zA-Z0-9]*#//g' | sort | uniq
RESULTS=$(curl $PHYLOWS_TREE_URI/$TREE_NAME | grep '<cdao\:Node' | grep -oE 'rdf\:resource=\"http://.*#[-_a-zA-Z0-9]*\">' | sed 's/>//g' | sed 's/rdf:resource=//g' | sed 's/\"//g' | sort | uniq)
#echo " <pre> $RESULTS </pre> "
ROW_STYLES[0]="even";
ROW_STYLES[1]="odd";

COUNT=0;
for i in $(echo $RESULTS ); do 
   nodeid=$(echo $i | sed 's/http:[-_~./a-zA-Z0-9]*#//g' )
   COUNT=$(( $COUNT + 1 ));
   STYLE=${ROW_STYLES[ $(( $COUNT % 2 )) ]}
   cat << EOM
      <tr class="$STYLE"><td>$nodeid</td><td><input type="checkbox" name="node" value="$nodeid"/></td></tr>
EOM
done
RESULT_SIZE=$COUNT
#$(echo $RESULTS | grep -oE "http" | wc -l)
echo "<tr><td colspan=\"2\"><input class=\"button\" type=\"submit\" value=\"Submit\"/><input class=\"button\" type=\"reset\" value=\"Reset\"/></td></tr>"
echo "</table></form>"
echo "<p>Results: $RESULT_SIZE</p>"
}

print_header;
print_results;
print_footer;
