#!/bin/bash

JOB="$1"
echo "Runing job: $JOB on host $(hostname)"
time (curl "$JOB" >/dev/null) 2>> `hostname`;
echo "$(hostname)"
