#!/bin/bash
# 
# File:   do_html_study_query.sh
# Author: bchisham
#
# Created on March 4, 2010, 12:20 AM
#

#Setup the environment
export ALGORITHM=`echo "$QUERY_STRING" | grep -oE "(^|[?&])algorithm=[^&]+" | sed "s/%20/ /g" | sed "s/+/ /g"  | cut -f 2 -d "="`
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"

#source ~/.bashrc
source ~/cdaostorecgi.rc


#Print the user form if the $STUDY variable is unset.
function print_form {
cat << EOM
   <table><form action="html" metod="get">
     <tr><td>Algorithm:</td><td><input type="text" class="text" id="algorithm" name="algorithm" value="$ALGORITHM"></td></tr>
     <tr><td colspan="2"><input class="button"type="submit" value="Submit"/><input class="button" type="reset" value="Reset"/><td></tr>
   </form></table>
EOM
}

function print_result {
   ALGORITHM_QUERY=$(do_algorithm_list_query_construct.sh "$ALGORITHM" )
   echo "<!-- ALGORITHM_QUERY: $ALGORITHM_QUERY -->"
   TMP_QUERY_FILE=`mktemp`".rql";
   echo "$ALGORITHM_QUERY" > $TMP_QUERY_FILE;
   
   RESULT_SIZE=0;
   echo "<h3>Trees Constructed with the \"$ALGORITHM\" Algorithm</h3>"
   echo "<div class=\"result-set\">"
   for result in $(sparql -q --results xml --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" | grep -oE "<uri>.*</uri>" | sort | cut -d">" --fields=2 | cut -d"<" --fields=1 | sed "s/$ESCAPED_DUMP_URI//g");
   do
        RESULT_SIZE=$[ $RESULT_SIZE + 1 ];
        echo "<a href=\"../tree/query?format=html&amp;tree=$result\">$result</a><br/>";
   done
   echo "</div>"
   echo "<p>Results: <span class=\"result-count\">$RESULT_SIZE</span></p>"
   #echo "</pre>";
   rm -f "$TMP_QUERY_FILE" 
}


print_header;

if [[ $ALGORITHM == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
