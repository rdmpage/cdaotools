#!/bin/bash
export CDAO_ROOT="/home/grad6/bchisham"
export LD_LIBRARY_PATH="$CDAO_ROOT/lib"
export PATH=$PATH:"$CDAO_ROOT/bin"
export CDAO_HOME="$CDAO_ROOT/bin"
#echo -e "$(whoami)"
#echo -e $LD_LIBRARY_PATH "<br/>\n"
#echo -e $PATH "<br/>\n"
#echo `ldd cdao-mass-import` "<br/>"

"$CDAO_HOME/cdao-mass-import" -i "$1"
perl -p -i -e 's/phylowsupload/phylows\/upload/g' "$1.cdao"
