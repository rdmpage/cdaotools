#!/bin/bash
#Brandon Chisham
#Feb. 19, 2010
#Script to show nca query results in html format.

export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/"
export TREE_DAT_FILE="~$(whoami)/bin/tree_to_file.dat"
export ANCESTOR_OF_FILE="~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl"
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$1"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"
export NODE_SET=`echo "$5" |  sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | sed "s/%20/ /g"`
export NODE_PATH=$( echo $NODE_SET | sed 's/.*#//g' | perl -p -n -e 's/ /\//g' )
export PHYLOWS_NCA="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/nca"
export PHYLOWS_TREE="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/tree"
NCA_NODE=$(curl "$PHYLOWS_NCA/$TREE_NAME/$NODE_PATH" | grep "Node" | grep -oE "http://[-_.~/#a-zA-Z0-9]*" )

cat << EOM
<?xml version="1.0" encoding="UTF-8"?>
 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml"
      xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
      xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
      xmlns:xsd="http://www.w3.org/2001/XMLSchema#"
      xmlns:cc="http://creativecommons.org/ns#"
      xmlns:dc="http://purl.org/dc/elements/1.1/"
      xmlns:foaf="http://xmlns.com/foaf/0.1/"
      xmlns:cdao="http://www.evolutionaryontology.org/cdao.owl#"
      xml:lang="en">
 <head profile="http://www.w3.org/1999/xhtml/vocab">
 <title>Cdao Store Query System</title>
      <link rel="stylesheet" type="text/css" href="../../style.css"/>
      <link rel="alternate" title="Project News" href="http://sourceforge.net/export/rss2_keepsake.php?group_id=282379" type="application/rss+xml"/>
      <link rel="SHORTCUT ICON" href="../cdao-store-favicon.ico" />
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 </head>
 <body about="http://www.evolutionaryontology.org/cdao.owl#">
 <div class="wrap-shadow">
 <div class="wrap" id="wrap">
 <div class="header">
  <h1 class="header"><a href="../../index.php"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a>Cdao Store
</h1>

 </div>
 <div class="content" style="scroll: auto;">

   <div resource="$XMLNS$TREE_NAME">
      <div resource="$NCA_NODE">
          In <a href="http://www.cs.nmsu.edu/~bchisham/cgi-bin/tree/query?format=html&amp;tree=$TREE_NAME">$TREE_NAME</a>, $NCA_NODE is the nearest common ancestor of....
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
<p about=""
     resource="http://www.w3.org/TR/rdfa-syntax"
     rel="dct:conformsTo" xmlns:dc="http://purl.org/dc/terms/">
    <a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-xhtml-rdfa-blue"
        alt="Valid XHTML + RDFa" style="border: 0px;"  /></a>
  </p>
     
   </div>
   </div>
   </div>
  </body>
</html>
EOM
