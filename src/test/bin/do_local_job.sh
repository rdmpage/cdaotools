#!/bin/bash

export SERVER_NAME="www.cs.nmsu.edu"
export REQUEST_URI="$1"
JOB="$2"
echo "REQUEST_URI=\"$REQUEST_URI\" Runing job: '$JOB' on host $(hostname)"
time (cd ~/public_html/cgi-bin/phylows && ./"$JOB" >/dev/null ) 2>> ~/`hostname`;
#time (cd ~/public_html/cgi-bin/phylows/ &&  ./"$JOB"  ) >> ~/`hostname`;
echo "$(hostname)"
