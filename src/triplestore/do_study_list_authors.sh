#!/bin/bash
# 
# File:   do_study_list_authors.sh
# Author: bchisham
#
# Created on February 27, 2010, 1:13 AM
#
GRAPH_BASE_URI="http://www.cs.nmsu.edu/~bchisham/dump_utf8.txt.owl#"
STUDYID="$1"

echo -e "PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
         PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
         PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
SELECT ?lname
WHERE {
    <$GRAPH_BASE_URI$STUDYID> study:has_author ?author .
    ?author foaf:last_name ?lname.
}"