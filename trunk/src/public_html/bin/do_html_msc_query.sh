#!/bin/bash
#Brandon Chisham
#Mar. 10, 2010
#Script to show msc query results in html format.
source ~/cdaostorecgi.rc

export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/"
export TREE_DAT_FILE="~$(whoami)/bin/tree_to_file.dat"
export ANCESTOR_OF_FILE="~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl"
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export NODE_PATH=$(echo $NODE_SET | sed 's/.*#//g' | perl -p -n -e 's/ /\//g' )
export PHYLOWS_MSC="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/msc"

function print_results {
MSC_NODES=$(curl "$PHYLOWS_MSC/$TREE_NAME/$NODE_PATH" | grep "has_Element" | grep  -oE "http://[-_.~#/a-zA-Z0-9]*")
rm -f "$TMP_RULES" "$PROLOG_OUT" #$TMP_GOAL

cat << EOM
   <div resource="$XMLNS$TREE_NAME">
      <div resource="">
           In <a href="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/tree/query?format=html&amp;tree=$TREE_NAME">$TREE_NAME</a> the minimum spannning clade of<br/> $NODE_SET
          <div rel="">
             <div resource="#node_set" style="position: relative; top: 0px; left: 30px;">
EOM
          RESULT_SIZE=0
          for i in $MSC_NODES; do 
                N=`echo $i | sed 's/.*#//g' | sed 's/_/ /g'`
                RESULT_SIZE=$[ $RESULT_SIZE + 1 ]
                echo "<div rel=\"cdao:has_Element\" href=\"$i\"><a href=\"http://www.google.com/search?q=$(echo $N | sed 's/ /%22/g')\">$N</a></div>"
          done 
cat << EOM
           <p>Results: $RESULT_SIZE</p>
         </div>
       </div>
     </div>
   </div>
EOM
}


print_header;
print_results;
print_footer;
