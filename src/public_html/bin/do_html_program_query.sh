#!/bin/bash
# 
# File:   do_html_program_query.sh
# Author: bchisham
#
# Created on March 4, 2010, 12:50 PM
#

#Setup the environment
export SOFTWARE=$(echo "$QUERY_STRING" |grep -oE "(^|[?&])software=[^&]+" | sed "s/%20/ /g" | sed "s/+/ /g" | cut -f 2 -d "=")
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
#export STUDY_RESULT_TRANSFORM="../../cdao-store/study_result_to_html.xsl"
#export STUDY_AUTHORS_TRANSFORM="../../cdao-store/study_authors_to_html.xsl"
export TREE_URI="../tree/query?format=html&amp;tree="
source ~/.bashrc
source ~/cdaostorecgi.rc

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
   export SOFTWARE_QUERY=$(print_query "$SOFTWARE");
   echo "<!-- SOFTWARE_QUERY: $SOFTWARE_QUERY -->"
   TMP_QUERY_FILE=`mktemp`".rql";
   echo "$SOFTWARE_QUERY" > $TMP_QUERY_FILE;
   echo "<h3>Trees Constructed with the $SOFTWARE Program</h3>"
   echo "<div class=\"result-set\">"
   ROW_COUNT=0;
   for result in $(sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g" | sort);
   do
        echo "$result <a href=\"../tree/query?format=html&amp;tree=$result\">Query</a> <a href=\"../../cdao-explorer/launch.php?tree=$result\">View</a><br/>";
        ROW_COUNT=$[ $ROW_COUNT + 1 ];
   done
   echo "<span class=\"result-count\">$ROW_COUNT</span>"
   #echo "</pre>";
   echo "</div>"
   rm -f "$TMP_QUERY_FILE" 
}


print_header;

if [[ "$SOFTWARE" == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
