#!/bin/bash
# 
# File:   do_study_detail_query_construct.sh
# Author: bchisham
#
# Created on February 26, 2010, 10:12 PM
#

GRAPH_BASE_URI="http://www.cs.nmsu.edu/~bchisham/dump_utf8.txt.owl#"
STUDYID="$1"

echo -e "PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
         PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
         PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
SELECT ?citation ?abstract
WHERE {
    <$GRAPH_BASE_URI$STUDYID> study:has_citation ?citation.
    <$GRAPH_BASE_URI$STUDYID> study:has_abstract ?abstract.
}"