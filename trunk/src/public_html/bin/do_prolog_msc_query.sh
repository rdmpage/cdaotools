#!/bin/bash

#Brandon Chisham
#Script to Answer Minimum Spanning Clade Queries.

#-->Params
#----->$1 database configuration string
#----->$2 tree name
#----->$3 xml base.
#----->$4 Type
#----->$5 NodeSet
export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/cdao-store/"
export TREE_DAT_FILE="~$(whoami)/bin/tree_to_file.dat"
export ANCESTOR_OF_FILE="~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl"
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"
export NODE_SET=`echo "$5" |  sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | sed "s/%20/ /g"`
export NCA_FILE="$6"

export NODE_QUERY=`do_tu_query_construct.sh "$XMLNS$TREE_NAME"`
export EDGE_QUERY=`do_edge_query_construct.sh "$XMLNS$TREE_NAME"`
export CTIME=`date`
export PL="/home/grad6/bchisham/bin/pl"
echo "#!$PL -q -t main -f"
echo -e "/* $NODE_QUERY */"
echo -e "/* $EDGE_QUERY */"
echo -e "/* $NCA_FILE */"
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

echo "/*NodeSet: $NODE_SET */"
#Put some meta information in the file.
echo "generator('cdao-store', '$CDAO_STORE_URI', '$CTIME')."
echo "namespace( '$TREE_NAME', '$XMLNS' )."
echo "namespace( 'cdao', '$CDAONS' )."

export CSV_NODE_SET=$(echo $NODE_SET | sed "s/ /','/g")
export NODE_COUNT=$[$( echo $CSV_NODE_SET | perl -p -n -e "s/,\'http/\n\'/g" | wc -l ) ]

echo "%NodeSet Size: $NODE_COUNT"

export MSC_QUERY="minimum_spanning_clade_of( '$TREE_NAME', NcaNode, Node )."
echo "%$MSC_QUERY"
#Get the facts associated with the specified tree.
TREE_FACTS=$(do_query.py "$GRAPH_CONFIG" "$XMLNS" "$NODE_QUERY" "node( '$TREE_NAME', '%s', '%s' )." "$EDGE_QUERY" "edge( '$TREE_NAME', '$TREE_TYPE', '%s', '%s', '%s')." |sort | uniq );
echo "$TREE_FACTS"

echo "%---Begin import of ancestor_of.pl----"
ANCESTOR_RULES=$(cat "$ANCESTOR_OF_FILE");
echo "$ANCESTOR_RULES"
echo "%---End import of ancestor_of.pl------"
echo "%---Begin import of nca_result--------"
NCA_RULES=`mktemp`;
#echo "$TREE_FACTS" > $NCA_RULES
do_prolog_nca_query.sh "$GRAPH_CONFIG" "$TREE_NAME" "$XMLNS" "$TYPE" "$NODE_SET" > $NCA_RULES;
NCA_OUT=`mktemp`;
QUERY_GOAL=`tail -n 1 $NCA_RULES`;
echo -e "consult('$NCA_RULES').\n$QUERY_GOAL\nhalt.\n\n" | pl -g "true" 2>"$NCA_OUT"
NCA_NODE=`cat "$NCA_OUT" | grep  -oE "'http.*'" | sed "s/'//g"`
rm -f "$NCA_RULES" "$NCA_OUT" #$TMP_GOAL
#NCA_RESULT=$(cat "$NCA_FILE");
NCA_FACT="nearest_common_ancestor_of( '$TREE_NAME', '$NCA_NODE', '$CSV_NODE_SET')."
echo "$NCA_FACT"
echo "%---End import of nca-result----------"
VAR_DEFS=""
for i in $(seq 1 "$NODE_COUNT"); do
   VAR_DEFS="$VAR_DEFS, N$i"
done

MSC_HEAD="minimum_spanning_clade_of( TreeName, NcaNode, Node ):- "

#MSC_RULE1="$MSC_HEAD nearest_common_ancestor_of( '$TREE_NAME', NcaNode '$CSV_NODE_SET' )."
MSC_RULE2="$MSC_HEAD nearest_common_ancestor_of( TreeName, NcaNode, '$CSV_NODE_SET' ), ancestor_of( TreeName,'$NCA_NODE', Node)."

echo "%---Begin dynamically generated rules---"
#echo $MSC_RULE1
echo $MSC_RULE2
#echo $DISTANT_COMMON_ANCESTOR_RULE
#echo $NEAREST_COMMON_ANCESTOR_RULE
MSC_QUERY="minimum_spanning_clade_of( '$TREE_NAME', '$NCA_NODE', Node)."
echo "%---End dynamically generated rules-----"
echo "%---Query-------------------------------"
echo "main :-"
echo $MSC_QUERY
