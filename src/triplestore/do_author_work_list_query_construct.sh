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
echo "SELECT ?study ?study 
       WHERE {
   ?study study:has_author ?authorid.
   ?authorid foaf:last_name '''$LAST_NAME'''.
}"

else 
#use both names in the query
echo "SELECT ?study ?study
WHERE {
   ?study study:has_author ?authorid.
   ?authorid foaf:last_name '''$LAST_NAME'''.
   ?authorid foaf:first_name '''$FIRST_NAME'''.
}"
fi
