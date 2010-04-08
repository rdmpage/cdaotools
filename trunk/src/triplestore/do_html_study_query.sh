#!/bin/bash
# 
# File:   do_html_study_query.sh
# Author: bchisham
#
# Created on February 26, 2010, 9:34 PM
#

#Setup the environment
export STUDY=`echo $QUERY_STRING |grep -oE "(^|[?&])study=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
export STUDY_RESULT_TRANSFORM="../../cdao-store/study_result_to_html.xsl"
export STUDY_AUTHORS_TRANSFORM="../../cdao-store/study_authors_to_html.xsl"
export AUTHOR_URI="../author/html?last="
source ~/.bashrc

export STUDY_QUERY=$( do_study_detail_query_construct.sh "$STUDY" )
export AUTHOR_QUERY=$( do_study_list_authors.sh "$STUDY" )
export TREE_QUERY=$( do_study_list_trees.sh "$STUDY" )

#Print the HTML header information
function print_header {
cat << EOM
Content-type: text/html; charset: utf-8


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
      <link rel="SHORTCUT ICON" href="../../../cdao-store-favicon.ico" />
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 </head>
 <body about="http://www.evolutionaryontology.org/cdao.owl">
   <div class="main-content" style="scroll: auto;">
     <h1>Cdao Store Query System</h1>
     <p><a href="../../cdao-store/index.html"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a></p>


EOM
}

#Print the html footer.
function print_footer {
cat << EOM
   </div>
</body>
</html>
EOM
}
#Print the user form if the $STUDY variable is unset.
function print_form {
cat << EOM
   <table><form action="html" metod="get">
     <tr><td>Study:</td><td><input type="text" id="study" name="study" value="$STUDY"></td></tr>
     <tr><td colspan="2"><input class="button"type="submit" value="Submit"/><input class="button" type="reset" value="Reset"/><td></tr>
   </form></table>
EOM
}

function print_result {
   echo "<!-- STUDY_QUERY: $STUDY_QUERY -->"
   echo "<!-- AUTHOR_QUERY: $AUTHOR_QUERY -->"
   echo "<!-- TREE_QUERY: $TREE_QUERY -->"
   echo "<h2>Listing detail for study id: $STUDY</h2>"
   TMP_QUERY_FILE=`mktemp`".rql";
   #TMP_RESULTS_FILE=`mktemp`".xml";
   TMP_AUTHOR_QUERY_FILE=`mktemp`".rql";
   TMP_TREE_QUERY_FILE=`mktemp`".rql"
   echo "$STUDY_QUERY" > $TMP_QUERY_FILE;
   echo "$AUTHOR_QUERY" > $TMP_AUTHOR_QUERY_FILE;
   echo "$TREE_QUERY" > $TMP_TREE_QUERY_FILE;
   #echo "<pre>";
   echo "<h3>Authors</h3>"
   sparql -q --results xml --query "$TMP_AUTHOR_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_AUTHORS_TRANSFORM -
   echo "<h3>Citation and Abstract</h3>"
   sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_RESULT_TRANSFORM - #| perl -p -n -e "s/^ *\n//g" | head -n 1
   echo "<h3>Trees</h3>"
   for result in $(sparql -q --results xml --query "$TMP_TREE_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g");
   do
        echo "<a href=\"../tree/query?format=html&amp;tree=$result\">$result</a><br/>";
   done
   #echo "</pre>";
   rm -f "$TMP_AUTHOR_QUERY_FILE" "$TMP_QUERY_FILE" "$TMP_TREE_QUERY_FILE" "$TMP_RESULTS_FILE"
}


print_header;

if [[ $STUDY == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
