#!/bin/bash

#Brandon Chisham
#May 8, 2010
#Script to produce ordered query results for the phylows service.
source ~/.bashrc
export FORMAT=$(echo "$QUERY_STRING" | grep -oE "format=(graphml|prolog|nexml|nexus|rdf|cdao)" | cut -d= --fields=2)

if [[ "$FORMAT" == "graphml" ]]; then
   do_phylows_unordered_tree.sh | order_graphml.sh
else
   do_"$FORMAT"_tree.sh
fi
