#!/bin/bash
#Brandon Chisham
#Feb. 14, 2010
#Script to generate a query to retrive all TU's from a set.

echo -e "SELECT ?tu ?node
         WHERE { 
	    ?node cdao:part_of <$1>.
	    ?tu cdao:represented_by_Node ?node . 
	 }"
