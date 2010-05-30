#!/bin/bash
#Script to list trees containing a given tu.

TU=`echo $QUERY_STRING | grep -oE "(^|[?&])tu=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`

STATIC_WEB_DIR="../../"
TRANSFORM="tu_to_filelist.xsl"
XSL="$STATIC_WEB_DIR/$TRANSFORM"
DATA=tu_to_filename.xml

source ~/cdaostorecgi.rc

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
  echo "<h3>Trees Containing the Specified Taxonomic Unit</h3>"
  echo "<div class=\"result-set\">"
  RESULTS=$(for i in $TU; do
               xsltproc --stringparam target "$i" "$XSL" "$DATA";
             done);
  
  RESULTS=$(echo $RESULTS | sort | uniq)
  RESULT_SIZE=$[$(echo $RESULTS | grep -oE "href" | wc -l ) / 2 ]
  echo $RESULTS
  echo "<p>Results: <span class=\"result-count\">$RESULT_SIZE</span></p>"
  echo "</div>"
}

print_header;
if [[ "$TU" == "" ]]; then
   print_form;
else
   print_results;
fi
print_footer;
