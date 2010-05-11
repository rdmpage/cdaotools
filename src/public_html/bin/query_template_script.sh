#!/bin/bash

#Brandon Chisham.
#Script to export results from the triplestore

##########Setup the Environment###############
#This is the URL prefix used by the cdao-import tool for id's declared in translated files.
export XML_BASE="http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows"
#This file contains file names and the tree names they contain.
export TREE_DAT_FILE=~$(whoami)/bin/tree_to_file.dat
#Extract parameters from the request
export TREE=`echo "$QUERY_STRING" | sed -n 's/^.*tree=\([^&]*\).*$/\1/p' | sed "s/%20/ /g" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" `;
#export TFILE=`echo "$QUERY_STRING" | sed -n 's/^.*file=\([^&]*\).*$/\1/p' | sed "s/%20/ /g" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g"`;
#export TREE_TYPE=`echo "$QUERY_STRING" | sed -n 's/^.*type=\([^&]*\).*$/\1/p' | sed "s/%20/ /g" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g"`;
export NODE_SET=`echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])node=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
#Support rest style request's extract the format type from the url
export RESPONSE_TYPE=`echo "$REQUEST_URI" | perl -p -n -e 's/^.*\/(prolog|html|graphml|nexml).*$/$1/'`;
#Extract query type from the url
export QUERY_TYPE=`echo "$REQUEST_URI" | perl -p -n -e 's/^.*\/(tree|tu|nca|msc)\/(prolog|html|graphml|nexml).*$/$1/'`;



#Load the db connection string.
if [[ "$TRIPLESTORE_CONFIG_STRING" == "" ]]; then
     source ~/triplestorerc
fi
#Setup the local python path if it's not done already
if [[ "$PYTHONPATH" == "" ]]; then
    source "~/.bashrc";
fi

#Print out the header information for the user form.
function htmlheader {
echo -e -n "Content-type: text/html; charset=utf-8\n\n"
cat << EOM 
 <!-- <?xml version="1.0"?> -->
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
      <link rel="SHORTCUT ICON" href="../../cdao-store-favicon.ico" />
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 </head>
 <body>
   <div class="main-content">
    <h1>Cdao Store Query System</h1>
EOM
}

#Print user entry form.

function printform {

cat << EOM
  <p>
    <a href="../../cdao-store/index.html"><img src="../../cdao-triplestore-logo.jpg" alt="Cdao Store Logo" style="border: 0px;" /></a>
  </p>

 <form id="TreeForm" method="get" action="$REQUEST_URI" enctype="application/x-www-form-urlencoded">
       <p>Tree: <input type="text" class="text" about="cdao:Tree" id="tree" name="tree" value="" />
         <input class="button" type="submit" value="Submit" />
         <input class="button" type="reset" value="Reset"/></p>
        </form>
<p about=""
     resource="http://www.w3.org/TR/rdfa-syntax"
          rel="dct:conformsTo" xmlns:dc="http://purl.org/dc/terms/">
	      <a href="http://validator.w3.org/check?uri=referer" ><img
	              src="http://www.w3.org/Icons/valid-xhtml-rdfa-blue"
		              alt="Valid XHTML + RDFa" style="border:0px;"   /></a>
			        </p>

EOM

#cat << EOM
#  <h2>Enter SparQL Query</h2>
#  <form id="query" action="http://www.cs.nmsu.edu/cgi-bin/sparql" method="post">
#      <textarea  id="sparql" style="height: 300px; width: 600px;"></textarea>
#      <br/>
#      <input type="submit" id="submit" class="button" value="Submit"/>
#      <input type="reset" id="reset" class="button" value="Reset"/>
#  </form>
#EOM

}

#Print graphml mime type for response
function responseheader {
if [[ "$RESPONSE_TYPE" == "prolog" ]]; then
  echo -e -n "Content-type: text/plain; charset=utf-8\n\n";
#  echo $RESPONSE_TYPE
else
  if [[ "$RESPONSE_TYPE" == "html" ]]; then
     echo -e -n "Content-type: text/html; charset=utf-8\n\n";
  else  
     echo -e -n "Content-type: text/xml; charset=utf-8\n\n";
  fi
fi
}

#do nothing. the graphml footer is output by the query processor.

function responsefooter {
  echo -n "";
}

#Close the html document.
function htmlfooter {
cat << EOM
    </div>
  </body>
 </html>
EOM
}

#######Main#######
#test if the form or result set should be displayed.

if [[ "$TREE" != "" ]]; then
   if [[ "$TFILE" == "" ]]; then
        export TFILE=$(grep "$TREE" "$TREE_DAT_FILE" | cut -d' ' --fields=1);
   fi
   if [[ "$TREETYPE" == "" ]]; then
       export TREE_TYPE=$( grep "$TREE" "$TREE_DAT_FILE" | cut -d' ' --fields=3 ); 
   fi
  responseheader;
 # echo $NODE_SET
#  echo "<p>running query:<br/>$SPARQL_QUERY</p>"
  ssh daedalus.cs.nmsu.edu "cd public_html/cgi-bin && do_'$RESPONSE_TYPE'_'$QUERY_TYPE'_query.sh \"$TRIPLESTORE_CONFIG_STRING\" \"$TREE\" \"$XML_BASE/$TFILE#\" \"$TREE_TYPE\" \"$NODE_SET\""
  responsefooter
else

  htmlheader;
  printform;
  htmlfooter;

fi

