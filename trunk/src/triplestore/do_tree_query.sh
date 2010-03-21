#!/bin/bash
#Script to forward parameter format to the REST style interface.
source /home/grad6/bchisham/.bashrc
FORMAT=$(echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])format=[^&]+" | sed "
s/%20/ /g"  | cut -f 2 -d "=")

TREE=$(echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])tree=[^&]+" | sed "
s/%20/ /g"  | cut -f 2 -d "="  )

function print_form {
cat << EOM
Content-type: text/html; charset: utf-8


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
    <h2>Select Tree</h2>
    <form action="query" method="get">
         <p>NeXML<input type="radio" name="format" value="nexml"/>
            GraphML<input type="radio" name="format" value="graphml"/>
            Prolog<input type="radio" name="format" value="prolog"/>
            HTML<input type="radio" name="format" value="html"/>
         </p>
         <p about="cdao:Tree"><span>Tree:</span><input type="text" class="text" id="tree" name="tree"/><br/>
            <input type="submit" value="Submit"/>
         </p>
    </form>
 </body>
</html>
EOM
}

if [[ "$FORMAT" != "" && "$TREE" != "" ]]; then
    export QUERY_STRING="?tree=$TREE"
    export REQUEST_URI="/tree/$FORMAT"
    query_template_script.sh 
else 
    print_form;
fi
