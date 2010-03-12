#!/bin/bash
#Script to list trees containing a given tu.

TU=`echo $QUERY_STRING | grep -oE "(^|[?&])tu=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`

STATIC_WEB_DIR="../../cdao-store"
TRANSFORM="tu_to_filelist.xsl"
XSL="$STATIC_WEB_DIR/$TRANSFORM"
DATA=tu_to_filename.xml

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


function print_form {
  cat << EOM
 <form id="TreeForm" method="get" action="html" enctype="application/x-www-form-urlencoded">
       <p>TU: <input type="text" class="text" about="cdao:Tree" id="tu" name="tu" value="" />
         <input class="button" type="submit" value="Submit" />
         <input class="button" type="reset" value="Reset"/></p>
        </form>
EOM
}

function print_results {
  RESULTS=$(for i in $TU; do
               xsltproc --stringparam target "$i" "$XSL" "$DATA";
             done);
  echo $RESULTS | sort | uniq;
}

function print_footer {
cat << EOM
<p about=""
     resource="http://www.w3.org/TR/rdfa-syntax"
          rel="dct:conformsTo" xmlns:dc="http://purl.org/dc/terms/">
	      <a href="http://validator.w3.org/check?uri=referer" ><img
	              src="http://www.w3.org/Icons/valid-xhtml-rdfa-blue"
		              alt="Valid XHTML + RDFa" style="border:0px;"   /></a>
			        </p>
  </div>
 </body>
</html>
EOM
}

print_header;
if [[ "$TU" == "" ]]; then
   print_form;
else
   print_results;
fi
print_footer;
