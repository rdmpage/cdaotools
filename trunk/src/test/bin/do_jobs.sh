#!/bin/bash

JOBS_FILE="$1"
JOB_NO=0;

for i in $(cat $JOBS_FILE); do
	time ("$i" >/dev/null) > `hostname`."$JOB_NO";
	JOB_NO=$[$JOB_NO + 1];
done
