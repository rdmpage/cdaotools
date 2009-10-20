#!/bin/bash
export PATH=$PATH:..:.;
for i in `ls test/*.nex` ; do
        echo "Processing " $i;
        ./run_cdao_import.pl -i $i -o `echo $i | sed "s/nex/owl/g"` ;
        echo "";
        done

