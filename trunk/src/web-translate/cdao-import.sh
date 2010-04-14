#!/bin/bash
export LD_LIBRARY_PATH="~tle/lib"
export PATH=$PATH:"~tle/bin"
#echo -e $LD_LIBRARY_PATH "<br/>\n"
#echo -e $PATH "<br/>\n"
#echo `ldd cdao-mass-import` "<br/>"

cdao-mass-import -i $1
