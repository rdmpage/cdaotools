#!/bin/bash

#Brandon Chisham
#Script to generate edge lookup query for a particular tree.
#Params
#------>$1 tree name (qualified)

export TREE_NAME="$1"

echo -e "SELECT ?edge ?src ?dest 
      WHERE 
      { 
         ?dest cdao:part_of <$TREE_NAME>.
         ?src cdao:part_of <$TREE_NAME>.
         ?edge cdao:has_Child_Node ?dest .
         ?edge cdao:has_Parent_Node ?src .
       }";
