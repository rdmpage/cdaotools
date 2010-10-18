#!/bin/bash

#Brandon Chisham
#Script to Answer NCA Queries.

#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 Type
#----->$5 NodeSet
export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/"
export TREE_DAT_FILE=~$(whoami)/bin/tree_to_file.dat
export ANCESTOR_OF_FILE=~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"
export NODE_SET=`echo $5 |  sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | sed "s/%20/ /g"`

export NODE_QUERY=`do_tu_query_construct.sh "$XMLNS$TREE_NAME"`
export EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`
export CTIME=`date`

echo -e "/*" $NODE_QUERY "*/"
echo -e "/*" $EDGE_QUERY "*/"
echo -e "/*" $XMLNS  "*/"
if [[ "$TFILE" == "" ]]; then
        export TFILE=$(grep "$TREE_NAME" "$TREE_DAT_FILE" | cut -d' ' --fields=1);
fi
if [[ "$TREETYPE" == "" ]]; then
       export TREE_TYPE=$( grep "$TREE_NAME" "$TREE_DAT_FILE" | cut -d' ' --fields=3 ); 
fi


echo -e "/*format: generator( 'system name', 'URI', 'date'  ).
                   namespace( 'shortname', 'URI' ).
                   node( TREE_NAME, TU_NAME, NODE_NAME). 
                   edge( TREE_NAME, TREE_TYPE, EDGE_NAME, PARENT, CHILD ).*/"

echo -e "%NodeSet: $NODE_SET"
#Put some meta information in the file.
echo "generator('cdao-store', '$CDAO_STORE_URI', '$CTIME')."
echo "namespace( '$TREE_NAME', '$XMLNS' )."
echo "namespace( 'cdao', '$CDAONS' )."

export CSV_NODE_SET=$(echo $NODE_SET | sed "s/ /','/g")
export NODE_COUNT=$[$( echo $CSV_NODE_SET | perl -p -n -e "s/,\'http/\n\'/g" | wc -l ) ]

echo "%NodeSet Size: $NODE_COUNT"

export NEAREST_ANCESTOR_QUERY="nearest_common_ancestor_of( '$TREE_NAME', Ancestor, '$CSV_NODE_SET')."
echo "%$NEAREST_ANCESTOR_QUERY"
#Get the facts associated with the specified tree.
do_query.py "$GRAPH_CONFIG" "$XMLNS" "$NODE_QUERY" "node( '$TREE_NAME', '%s', '%s' )." "$EDGE_QUERY" "edge( '$TREE_NAME', '$TREE_TYPE', '%s', '%s', '%s')."

echo "%---Begin import of ancestor_of.pl----"
cat $ANCESTOR_OF_FILE
echo "%---End import of ancestor_of.pl------"

COMMON_ANCESTOR_PRED="common_ancestor_of( Tree, Ancestor "
DISTANT_COMMON_ANCESTOR_PRED="distant_common_ancestor_of(Tree, Ancestor "
NEAREST_COMMON_ANCESTOR_PRED="nearest_common_ancestor_of(Tree, Ancestor "
COMMON_ANCESTOR_TAIL=":- "
#DISTANT_COMMON_ANCESTOR_TAIL=":- "
for i in $(seq 1 $NODE_COUNT); do
     COMMON_ANCESTOR_PRED="$COMMON_ANCESTOR_PRED, N$i "
     DISTANT_COMMON_ANCESTOR_PRED="$DISTANT_COMMON_ANCESTOR_PRED, N$i ";
     NEAREST_COMMON_ANCESTOR_PRED="$NEAREST_COMMON_ANCESTOR_PRED, N$i ";
     COMMON_ANCESTOR_TAIL="$COMMON_ANCESTOR_TAIL ancestor_of( Tree, Ancestor, N$i ),"
done
COMMON_ANCESTOR_PRED="$COMMON_ANCESTOR_PRED)"
DISTANT_COMMON_ANCESTOR_PRED="$DISTANT_COMMON_ANCESTOR_PRED )"
NEAREST_COMMON_ANCESTOR_PRED="$NEAREST_COMMON_ANCESTOR_PRED )"
COMMON_ANCESTOR_TAIL=`echo $COMMON_ANCESTOR_TAIL | sed 's/,$/./'`
DISTANT_COMMON_ANCESTOR_TAIL=":- `echo $COMMON_ANCESTOR_PRED | sed 's/Ancestor/IntermediateCommonAncestor/g'`, ancestor_of( Tree, Ancestor, IntermediateCommonAncestor )."
NEAREST_COMMON_ANCESTOR_TAIL=":- $COMMON_ANCESTOR_PRED, \+ $DISTANT_COMMON_ANCESTOR_PRED."
COMMON_ANCESTOR_RULE="$COMMON_ANCESTOR_PRED $COMMON_ANCESTOR_TAIL"
DISTANT_COMMON_ANCESTOR_RULE="$DISTANT_COMMON_ANCESTOR_PRED $DISTANT_COMMON_ANCESTOR_TAIL"
NEAREST_COMMON_ANCESTOR_RULE="$NEAREST_COMMON_ANCESTOR_PRED $NEAREST_COMMON_ANCESTOR_TAIL"

echo "%---Begin dynamically generated rules---"
echo $COMMON_ANCESTOR_RULE
echo $DISTANT_COMMON_ANCESTOR_RULE
echo $NEAREST_COMMON_ANCESTOR_RULE
echo "%---End dynamically generated rules-----"
echo "%---Query-------------------------------"
echo $NEAREST_ANCESTOR_QUERY
