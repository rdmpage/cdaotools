#!/bin/bash

#Brandon Chisham
#Script to generate study lookup query for a particular author.
#Params
#------>$1 last name
#------>$2 first name (optional)

LAST_NAME="$1"
FIRST_NAME="$2"

if [[ $FIRST_NAME == "" ]]; then
#use only last name
echo "PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
      PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
      PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
   SELECT ?study
       WHERE {
   ?study study:has_author ?authorid.
   ?authorid foaf:last_name '$LAST_NAME'. }"

else 
#use both names in the query
echo "PREFIX study: <http://www.cs.nmsu.edu/~bchisham/study.owl#>
      PREFIX contact: <http://www.w3.org/2000/10/swap/pim/contact#>
      PREFIX foaf: <http://www.mindswap.org/2003/owl/foaf#>
SELECT ?study
WHERE {
   ?study study:has_author ?authorid.
   ?authorid foaf:last_name '$LAST_NAME'^^<http://www.w3.org/2001/XMLSchema#string>.
   ?authorid foaf:first_name '$FIRST_NAME'^^<http://www.w3.org/2001/XMLSchema#string>.}"
fi
