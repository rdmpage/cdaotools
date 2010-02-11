#!/bin/bash
#Brandon Chisham
#Feb. 11, 2010

#Build a mapping from filenames in a treebase dump to (tree name, rooted/unrooted).

export URL="http://www.cs.nmsu.edu/~epontell"
echo "<?xml version=\"1.0\"?>"
echo "<files>"
for i in $(ls *.tre); 
do 
        echo "<file id=\"$URL/$i\">"; 
        grep "^TREE" $i | sed 's/TREE //' | perl -p -n -e 's/([a-zA-Z0-9]+)/<tree id=\"$1\" /' |sed 's/\[&R\]/ type=\"directional\" /' | sed 's/\[&U\]/type=\"undirectional\" /' | sed 's/END;//' | sed 's/= //g' | sed 's/  / /g' | perl -p -n -e 's/ (\(.*\)(?:[_a-zA-Z0-9]+)?);/\/>/g'; 
        echo "</file>"
done
#Use to add actual tree data as "newick" attribute
#perl -p -n -e 's/ (\(.*\)(?:[_a-zA-Z0-9]+)?);/ newick=\"$1\"\/>/g';

echo "</files>"
