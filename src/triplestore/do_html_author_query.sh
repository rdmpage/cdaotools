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
export cdao="http://www.evolutionaryontology.org/cdao.owl"
export CDAONS="$cdao#"
export GRAPH_CONFIG="$TRIPLESTORE_CONFIG_STRING"
export GRAPH_FILE="../../dump_utf8.txt.owl"
export ESCAPED_DUMP_URI="http:\/\/www.cs.nmsu.edu\/~bchisham\/dump_utf8.txt.owl#"
export STUDY_URI="http://www.cs.nmsu.edu/~bchisham/cgi-bin/study/html?study="
#export TREE_NAME="$2"
#export XMLNS="$3"
#export TYPE="$4"

source ~/.bashrc

export FIRST_NAME=`echo $QUERY_STRING |grep -oE "(^|[?&])first=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`

export LAST_NAME=`echo $QUERY_STRING | grep -oE "(^|[?&])last=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`



export AUTHOR_QUERY=$(do_author_work_list_query_construct.sh "$LAST_NAME" "$FIRST_NAME");

export XMLNS="http://www.cs.nmsu.edu/~bchisham/study.owl#"


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
      <link rel="stylesheet" type="text/css" href="../../../style.css"/>
      <link rel="alternate" title="Project News" href="http://sourceforge.net/export/rss2_keepsake.php?group_id=282379" type="application/rss+xml"/>
      <link rel="SHORTCUT ICON" href="../../../cdao-store-favicon.ico" />
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 </head>
 <body about="http://www.evolutionaryontology.org/cdao.owl">
   <div class="main-content" style="scroll: auto;">
     <h1>Cdao Store Query System</h1>
     <p><a href="../../../cdao-store/index.html"><img src="../../../cdao-triplestore-logo.jpg" alt="Cdao-Store Logo" style="border: 0px;" /></a></p>


EOM
}


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
   TMP_QUERY_FILE=`mktemp`".rql";
   echo "$AUTHOR_QUERY" > $TMP_QUERY_FILE;
   echo "<p>";
   sparql -q --results text --query "$TMP_QUERY_FILE" --graph "$GRAPH_FILE" 
      
   echo "</p>";
   rm -f $TMP_QUERY_FILE
}


function print_footer {
cat << EOM
   </div>
</body>
</html>
EOM
}

print_header;

if [[ $LAST_NAME == "" ]]; then
   print_form;
else
   print_result;
fi

print_footer;
