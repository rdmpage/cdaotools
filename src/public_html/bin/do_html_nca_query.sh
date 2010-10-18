#!/bin/bash
#Brandon Chisham
#Feb. 19, 2010
#Script to show nca query results in html format.

source ~/cdaostorecgi.rc

export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/"
export TREE_DAT_FILE="~$(whoami)/bin/tree_to_file.dat"
export ANCESTOR_OF_FILE="~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl"
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export NODE_PATH=$( echo "$NODE_SET" | sed 's/.*#//g' | perl -p -n -e 's/ /\//g' )
export PHYLOWS_NCA="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/nca"
export PHYLOWS_TREE="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree"


NCA_NODE=$(curl "$PHYLOWS_NCA/$TREE_NAME/$NODE_PATH" | grep "Node" | grep -oE "http://[-_.~/#a-zA-Z0-9]*" )

function print_results {
   cat <<EOM
   <!-- $NODE_SET -->
   <!-- $NODE_PATH -->
   <div resource="$XMLNS">
      <div resource="$NCA_NODE">
          In <a href="http://www.cs.nmsu.edu/$(whoami)/cgi-bin/tree/query?format=html&amp;tree=$TREE_NAME">$TREE_NAME</a>, $NCA_NODE is the nearest common ancestor of....
          <div rel="cdao:nca_node_of">
             <div resource="#node_set" style="position: relative; top: 0px; left: 30px;">
             $(
               for i in $NODE_SET; do 
                  N=$(echo $i | sed 's/.*#//g' | sed 's/_/ /g')
                  echo "<p rel=\"cdao:has_Element\" href=\"$PHYLOWS_TREE/$TREE_NAME#$i\"><a href=\"http://www.google.com/search?q=$(echo $N | sed 's/_/%22/g' )\">$N</a></p>"
                done 
              )
         </div>
       </div>
     </div>
   </div>
<p>
   <a href="$CDAO_STORE_URI">Home</a><br/>
   <a href="../tree/query?format=html&amp;tree=$TREE_NAME">Find NCA of another Set</a>
</p>
EOM
}

print_header;
print_results;
print_footer;
