#!/bin/bash
# 
# File:   do_html_study_query.sh
# Author: bchisham
#
# Created on March 4, 2010, 12:50 PM
#

#Setup the environment
export SOFTWARE=`echo $QUERY_STRING |grep -oE "(^|[?&])software=[^&]+" | sed "s/%20/ /g" | sed "s/+/ /g" | cut -f 2 -d "="`
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
#export STUDY_RESULT_TRANSFORM="../../cdao-store/study_result_to_html.xsl"
#export STUDY_AUTHORS_TRANSFORM="../../cdao-store/study_authors_to_html.xsl"
export TREE_URI="../tu/html?last="
source ~/.bashrc

#export ALGORITHM_QUERY=$( do_algorithm_list_query_construct.sh "$ALGORITHM" )
#export AUTHOR_QUERY=$( do_study_list_authors.sh "$STUDY" )
#export TREE_QUERY=$( do_study_list_trees.sh "$STUDY" )


function print_query {
ALGO="$1"
cat << EOM
PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
SELECT ?tree 
WHERE {
    ?study study:has_analysis ?analysis .
    ?analysis study:has_software '$ALGO' .
    ?analysis study:has_output_tree ?tree .
}
  

EOM
}

export SOFTWARE_QUERY=`print_query $SOFTWARE`;

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
   <div class="wrap">
     <div class="header">
       <h1 class="header"><a href="../../cdao-store/index.html"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a>Cdao Store Query System</h1>
     </div>
   <div class="main-content" style="scroll: auto;">
EOM
}

#Print the html footer.
function print_footer {
cat << EOM
     </div>
   </div>
</body>
</html>
EOM
}
#Print the user form if the $SOFTWARE variable is unset.
function print_form {
cat << EOM
   <table><form action="html" metod="get">
     <tr><td>Algorithm:</td><td><input type="text" class="text" id="software" name="software" value="$SOFTWARE"></td></tr>
     <tr><td colspan="2"><input class="button"type="submit" value="Submit"/><input class="button" type="reset" value="Reset"/><td></tr>
   </form></table>
EOM
}

function print_result {
   echo "<!-- SOFTWARE_QUERY: $SOFTWARE_QUERY -->"
   #echo "<!-- AUTHOR_QUERY: $AUTHOR_QUERY -->"
   #echo "<!-- TREE_QUERY: $TREE_QUERY -->"
   #echo "<h2>Listing detail for study id: $STUDY</h2>"
   TMP_QUERY_FILE=`mktemp`".rql";
   #TMP_RESULTS_FILE=`mktemp`".xml";
   #TMP_AUTHOR_QUERY_FILE=`mktemp`".rql";
   #TMP_TREE_QUERY_FILE=`mktemp`".rql"
   echo "$SOFTWARE_QUERY" > $TMP_QUERY_FILE;
  # echo "$AUTHOR_QUERY" > $TMP_AUTHOR_QUERY_FILE;
  # echo "$TREE_QUERY" > $TMP_TREE_QUERY_FILE;
   #echo "<pre>";
   #echo "<h3>Authors</h3>"
   #sparql -q --results xml --query "$TMP_AUTHOR_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_AUTHORS_TRANSFORM -
   #echo "<h3>Citation and Abstract</h3>"
   #sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | xsltproc $STUDY_RESULT_TRANSFORM - #| perl -p -n -e "s/^ *\n//g" | head -n 1
   echo "<h3>Trees Constructed with the $SOFTWARE Program</h3>"
   echo "<div class=\"result-set\">"
   ROW_COUNT=0;
   for result in $(sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g" | sort);
   do
        echo "<a href=\"../tree/query?format=html&amp;tree=$result\">$result</a><br/>";
        ROW_COUNT=$[ $ROW_COUNT + 1 ];
   done
   echo "<span class=\"result-count\">$ROW_COUNT</span>"
   #echo "</pre>";
   echo "</div>"
   rm -f "$TMP_QUERY_FILE" 
}


print_header;

if [[ $SOFTWARE == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
