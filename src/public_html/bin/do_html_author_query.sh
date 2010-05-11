#!/bin/bash

#Script to dump nodes in GraphML Format
#Brandon Chisham
#Feb. 11, 2010
#Description:
#-->Params
#----->$1 database configuration string
#----->$2 Author Name.
#----->$3 xml base.
#----->$4 directed/undirected 
#Note do_query.py expects DBCONFIG_STRING QUERY_STRING FORMAT_STRING XML_BASE_URI
#export TREE_DAT_FILE=~bchisham/tree_to_file.dat
source ~/.bashrc

export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$TRIPLESTORE_CONFIG_STRING"
export GRAPH_FILE="http://www.cs.nmsu.edu/~$(whoami)/dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
export STUDY_URI="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/study/html?study="
export FIRST_NAME=`echo $QUERY_STRING |grep -oE "(^|[?&])first=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
export LAST_NAME=`echo $QUERY_STRING | grep -oE "(^|[?&])last=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
export AUTHOR_QUERY=$(do_author_work_list_query_construct.sh "$LAST_NAME" "$FIRST_NAME");
export XMLNS="http://www.cs.nmsu.edu/~bchisham/study.owl#"
source ~/cdaostorecgi.rc


function print_form {
cat << EOM
   <table><form action="html" metod="get">
     <tr><td>First:</td><td><input type="text" id="first" name="first" value="$FIRST_NAME">(optional)</td></tr>
     <tr><td>Last:</td><td><input type="text" id="last" name="last" value="$LAST_NAME"/></td></tr>
     <tr><td colspan="2"><input class="button"type="submit" value="Submit"/><input class="button" type="reset" value="Reset"/><td></tr>
   </form></table>
EOM

}


function print_result {
   echo "<!-- AUTHOR_QUERY: $AUTHOR_QUERY -->"
   echo "<h2>Listing studies for: $FIRST_NAME $LAST_NAME</h2>"
   echo "<div class=\"result-set\">"
   TMP_QUERY_FILE=`mktemp`".rql";
   echo "$AUTHOR_QUERY" > $TMP_QUERY_FILE;
   RESULT_SIZE=0;
   echo "<p>";
   for result in $(sparql -q --results text --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE"  | grep -oE "<.*>" | sed "s/<$ESCAPED_DUMP_URI\(.*\)>/\1/g" );
   do
        RESULT_SIZE=$[ $RESULT_SIZE + 1 ]
        echo "<a href=\"../study/html?study=$result\">$result</a><br/>";
   done
   echo "</p>";
   echo "</div>"
   echo "<p>Results: <span class=\"result-count\">$RESULT_SIZE</span></p>"
   rm -f $TMP_QUERY_FILE
}


print_header;

if [[ $LAST_NAME == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
