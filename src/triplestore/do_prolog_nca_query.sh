#!/bin/bash

#Brandon Chisham
#Script to Answer NCA Queries.

#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 Type
#----->$5 NodeSet
export TREE_DAT_FILE=~bchisham/bin/tree_to_file.dat
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

echo "generator('cdao-store', '$CDAO_STORE_URI', '$CTIME')."
echo "namespace( '$TREE_NAME', '$XMLNS' )."
echo "namespace( 'cdao', '$CDAONS' )."

export NEAREST_ANCESTOR_QUERY="nearest_common_ancestor_of( '$TREE_NAME', 'Ancestor', '$(echo $NODE_SET | sed "s/ .*http/','http/g")')."
echo "%$NEAREST_ANCESTOR_QUERY"
#Get the facts associated with the specified tree.
do_query.py "$GRAPH_CONFIG" "$XMLNS" "$NODE_QUERY" "node( '$TREE_NAME', '%s', '%s' )." "$EDGE_QUERY" "edge( '$TREE_NAME', '$TREE_TYPE', '%s', '%s', '%s')."
