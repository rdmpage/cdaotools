#!/bin/bash
#Brandon Chisham
#March 4, 2010
#Script to generate query for a particular algorithm type.

ALGO="$1"

echo -e "PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
         PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
         PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
SELECT ?tree 
WHERE {
    ?study study:has_analysis ?analysis.
    ?analysis study:has_algorithm '$ALGO' .
    ?analysis study:has_output_tree ?tree .
}"
