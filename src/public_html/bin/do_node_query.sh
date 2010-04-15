#!/bin/bash
#Script to forward parameter format to the REST style interface.
source /home/grad6/bchisham/.bashrc
#FORMAT=$(echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])format=[^&]+" | sed " s/%20/ /g"  | cut -f 2 -d "=")


#TREE=$(echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])tree=[^&]+" | sed "s/%20/ /g"  | cut -f 2 -d "="  )

QTYPE=$(echo "$QUERY_STRING" | sed "s/%3A/:/g" | sed "s/%2F/\//g" | sed "s/%7E/~/g" | sed "s/%23/#/g" | grep -oE "(^|[?&])qtype=[^&]+" | sed " s/%20/ /g"  | cut -f 2 -d "=")


if [[ "$QTYPE" != "" ]]; then
   # export QUERY_STRING="?tree=$TREE"
    export REQUEST_URI="/$QTYPE/html"
    query_template_script.sh 
fi
