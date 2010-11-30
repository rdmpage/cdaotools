#/bin/bash
export PATH="~/bin:$PATH"
export HOSTS_FILE="$1"
export JOBS_FILE="$2"

#scp do_jobs.sh daedalus.cs.nmsu.edu:~/bin/
#ssh daedalus.cs.nmsu.edu "chmod 700 ~/bin/do_jobs.sh"
exec 3< $JOBS_FILE

while read -r -a job <&3; do
	echo "Starting on job: ${job[1]}"
	for host in $(cat $HOSTS_FILE); do
		echo "Runing on host $host"
	        time ssh "$host" "do_local_job.sh '${job[0]}' '${job[1]}'"
	done
done
