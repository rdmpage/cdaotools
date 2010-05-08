#!/bin/bash

#Brandon Chisham
#May 8, 2010
#Script to produce ordered query results for the phylows service.
source ~/.bashrc
export FORMAT=$(echo "$QUERY_STRING" | grep -oE "format=[a-zA-Z][a-zA-Z]*" | cut -d= --fields=2)

if [[ "$FORMAT" == "graphml" ]]; then
   do_phylows_unordered_tree.sh | order_graphml.sh
else
   do_phylows_unordered_tree.sh
fi
