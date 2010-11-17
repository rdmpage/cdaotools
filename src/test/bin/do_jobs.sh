#!/bin/bash

JOBS_FILE="$1"
JOB_NO=0;

for i in $(cat $JOBS_FILE); do
	time (curl "$i" >/dev/null) 2>> `hostname`;
	JOB_NO=$[$JOB_NO + 1];
done
