#!/bin/bash
#Script to list trees containing a given tu.

CRITERION=`echo $QUERY_STRING | grep -oE "(^|[?&])criterion=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
SIZE=`echo $QUERY_STRING | grep -oE "(^|[?&])size=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`
DIRECTION=`echo $QUERY_STRING | grep -oE "(^|[?&])direction=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="`

PHYLOWS_SIZE="http://www.cs.nmsu.edu/~$(whoami)/cgi-bin/phylows/size"

source ~/cdaostorecgi.rc

function print_form {
  cat << EOM
 <form id="TreeForm" method="get" action="size" enctype="application/x-www-form-urlencoded">
       <table>
          <tr><td>Criterion:</td><td>node <input type="radio" class="text" about="cdao:Tree" id="criterion" name="criterion" value="node" /><br/>
				     internal <input type="radio" class="text" about="cdao:Tree" id="criterion" name="criterion" value="internal" /><br/>
				     leaf <input type="radio" class="text" about="cdao:Tree" id="criterion" name="criterion" value="leaf" />
                                </td></tr>
          <tr><td>Direction:</td><td>less <input type="radio" class="text" id="direction" name="direction" value="less"/><br/>
				     greater <input type="radio" class="text" id="direction" name="direction" value="greater"/><br/>
				     equal <input type="radio" class="text" id="direction" name="direction" value="equal"/><br/>

				</td></tr>
          <tr><td>Size:</td>     <td> <input type="text" class="text" id="size" name="size" value=""/></td></tr>
          <tr><td colspan="2"><input class="button" type="submit" value="Submit" /><input class="button" type="reset" value="Reset"/></td></tr>
        </table>
        </form>
EOM
}

function print_results {
  echo "<h3>Trees Matching The Specified Criterion</h3>"
  echo "<div class=\"result-set\">"
  RESULTS=$(curl "$PHYLOWS_SIZE/$CRITERION/$DIRECTION/$SIZE" | grep  "Tree" | grep -oE "http://[-~a-zA-Z0-9./]*");
  
  RESULTS=$(echo "$RESULTS" | sort | uniq)

  for i in `echo "$RESULTS"`; do
     tree=`echo $i | grep -oE '[a-zA-Z0-9][-_a-zA-Z0-9]*$'`
     echo "$tree <a href=\"../tree/query?format=html&amp;tree=$tree\">Query</a> <a href=\"../../cdao-explorer/launch.php?tree=$tree\">View</a><br/>"
  done 

  RESULT_SIZE=$[$(echo "$RESULTS" | grep -oE "http" | wc -l ) ]
  #echo $RESULTS
  echo "<p>Results: <span class=\"result-count\">$RESULT_SIZE</span></p>"
  echo "</div>"
}

print_header;
if [[ "$CRITERION" == "" || "$SIZE" == "" || "$DIRECTION" == "" ]]; then
   print_form;
else
   print_results;
fi
print_footer;
