#!/bin/bash

export TOTAL_TREES=`ls trees/*.tre | wc -l`;
export TOTAL_MATRICES=`ls characters/*.nex | wc -l`;
export trees_processed=0;
export matrices_processed;
for i in `ls trees/*.tre`; do 
        export cdao_file=`echo $i | sed 's/.tre/.cdao/g'`;
        export trees_processed=$[ $trees_processed + 1 ];
        if [[ -f $cdao_file ]]; then
                echo "Already processed: $cdao_file";
        else
                echo "Processing($trees_processed of $TOTAL_TREES ): $i result file: $cdao_file"; 
                cdao-import -i $i -o $cdao_file;
        fi
done

for i in `ls characters/*.nex`; do
        export cdao_file=`echo $i | sed 's/.nex/.cdao/g'`;
        export matrices_processed=$[ $matrices_processed + 1 ]; 
        if [[ -f $cdao_file ]]; then
                echo "Already translated: $cdao_file";
        else
                echo "Processing ($matrices_processed of $TOTAL_MATRICES): $i result file $cdao_file"; 
                cdao-import -i $i -o $cdao_file;
        fi 
done
