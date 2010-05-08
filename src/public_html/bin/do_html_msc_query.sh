#!/bin/bash
#Brandon Chisham
#Mar. 10, 2010
#Script to show msc query results in html format.

export CDAO_STORE_URI="http://www.cs.nmsu.edu/~$(whoami)/"
export TREE_DAT_FILE="~$(whoami)/bin/tree_to_file.dat"
export ANCESTOR_OF_FILE="~$(whoami)/public_html/cgi-bin/nca/ancestor_of.pl"
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$TRIPLESTORE_CONFIG_STRING"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"
export UNPARSED_NODE_SET="$5"
export NODE_SET=`echo "$5" |  sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | sed "s/%20/ /g"`
export NODE_PATH=$(echo $NODE_SET | sed 's/.*#//g' | perl -p -n -e 's/ /\//g' )
export PHYLOWS_MSC="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/msc"

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
 <div class="wrap" id="wrap">
 <div class="header" id="header">
   <h1 class="header"><a class="header" href="../../index.php"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a>Cdao Store Query System</h1>
 </div>
 <div id="content" class="content" style="scroll: auto;">
EOM

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

cat << EOM
<p>
   <a href="$CDAO_STORE_URI">Home</a><br/>
   <a href="../tree/query?format=html&amp;tree=$TREE_NAME">Find MSC of another Set</a>
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
  </body>
</html>
EOM
