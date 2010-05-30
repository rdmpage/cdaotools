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
export STUDY_RESULT_TRANSFORM="../../study_result_to_html.xsl"
export STUDY_AUTHORS_TRANSFORM="../../study_authors_to_html.xsl"
export AUTHOR_URI="../author/html?last="
source ~/cdaostorecgi.rc

export STUDY_QUERY=$( do_study_detail_query_construct.sh "$STUDY" )
export AUTHOR_QUERY=$( do_study_list_authors.sh "$STUDY" )
export TREE_QUERY=$( do_study_list_trees.sh "$STUDY" )


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
   echo "<div class=\"result-set\">"
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
        echo "$result <a href=\"../tree/query?format=html&amp;tree=$result\">Query</a> <a href=\"../../cdao-explorer/launch.php?tree=$result\"></a><br/>";
   done
   echo "</div>"
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
