#!/bin/bash
#Brandon Chisham
#Mar. 10, 2010
#Script to show msc query results in html format.

export CDAO_STORE_URI="http://www.cs.nmsu.edu/~bchisham/cdao-store/"
export TREE_DAT_FILE=~bchisham/bin/tree_to_file.dat
export ANCESTOR_OF_FILE=~bchisham/public_html/cgi-bin/nca/ancestor_of.pl
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$TRIPLESTORE_CONFIG_STRING"
export TREE_NAME="$2"
export XMLNS="$3"
export TYPE="$4"
export UNPARSED_NODE_SET="$5"
export NODE_SET=`echo "$5" |  sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | sed "s/%20/ /g"`



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
 <div class="main-content" style="scroll: auto;">
 <h1>Cdao Store Query System</h1>
 <p><a href="../../cdao-store/index.html"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a></p>
EOM

export TMP_RULES=`mktemp`".pl";
#export TMP_GOAL=`mktemp`;
do_prolog_msc_query.sh "$GRAPH_CONFIG" "$TREE_NAME" "$XMLNS" "$TYPE" "$UNPARSED_NODE_SET" > $TMP_RULES
#RULES_LEN=$(wc -l $TMP_RULES)
#PRED_LEN=$[ $RULES_LEN - 1 ]
CONSULT="consult( '$TMP_RULES' )."
QUERY_GOAL=$(tail -n 1 $TMP_RULES)

PROLOG_EXE=`mktemp`

#echo "$CONSULT" > $TMP_GOAL
#echo "$QUERY_GOAL" >> $TMP_GOAL
#echo "halt( 0 )." >> $TMP_GOAL
#echo -e "\n" >> $TMP_GOAL
PROLOG_OUT=`mktemp`.prolog.out

pl -o "$PROLOG_EXE" -c "$TMP_RULES"

prolog_exe_driver "$PROLOG_EXE" "$TMP_GOAL" >"$PROLOG_OUT"
MSC_NODE=`cat "$PROLOG_OUT" | grep  -oE "'http.*'" | sed "s/'//g"`
rm -f "$TMP_RULES" "$PROLOG_OUT" #$TMP_GOAL

cat << EOM
   <!-- 
         GOAL: "$QUERY_GOAL"
         RULES: "$TMP_RULES"
         PROLOG_OUT: "$PROLOG_OUT"
    -->
   <div resource="$XMLNS$TREE_NAME">
      <div resource="">
          In $TREE_NAME the minimum spannning clade of $NODE_SET
          <div rel="">
             <div resource="#node_set" style="position: relative; top: 0px; left: 30px;">
EOM
          for i in $NODE_SET; do 
                N=`echo $i | sed 's/.*#//g'`
                echo "<p rel=\"cdao:has_Element\" href=\"$i\"><a href=\"http://www.google.com/search?q=$N\">$N</a></p>"
          done 
cat << EOM
         </div>
       </div>
     </div>
   </div>
EOM

cat << EOM
<p>
   <a href="$CDAO_STORE_URI">Home</a><br/>
   <a href="../tree/html?tree=$TREE_NAME">Find MSC of another Set</a>
</p>
<p about=""
     resource="http://www.w3.org/TR/rdfa-syntax"
     rel="dct:conformsTo" xmlns:dc="http://purl.org/dc/terms/">
    <a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-xhtml-rdfa-blue"
        alt="Valid XHTML + RDFa" style="border: 0px;"  /></a>
  </p>
     
   </div>
  </body>
</html>
EOM
