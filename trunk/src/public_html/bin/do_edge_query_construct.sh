#!/bin/bash

echo -e "SELECT ?edge ?src ?dest
      WHERE 
      { 
         ?dest cdao:part_of <$1>.
         ?src cdao:part_of <$1>.
         ?edge cdao:has_Child_Node ?dest .
         ?edge cdao:has_Parent_Node ?src .
       }";
