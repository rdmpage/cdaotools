#!/bin/bash

echo -e "SELECT ?node ?node
      WHERE {
              {
                ?node cdao:part_of <$1>.  
                ?node rdf:type cdao:Node . 
              } 
              UNION 
              { 
                ?node cdao:part_of <$1>. 
                ?edge cdao:has_Child_Node ?node.  
              } 
              UNION 
              { 
                ?node cdao:part_of <$1>. 
                ?edge cdao:has_Parent_Node ?node. 
              } 
            }";

