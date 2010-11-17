#/bin/bash

export HOSTS_FILE="$1"
export JOBS_FILE="$2"


for host in $(cat $HOSTS_FILE); do
	for job in $(cat $JOBS_FILE); do
		(ssh "$host" && time ($job >/dev/null ) > "$host.$job.time");
	done
done
