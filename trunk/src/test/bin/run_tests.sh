#/bin/bash
export PATH="~/bin:$PATH"
export HOSTS_FILE="$1"
export JOBS_FILE="$2"

#scp do_jobs.sh daedalus.cs.nmsu.edu:~/bin/
#ssh daedalus.cs.nmsu.edu "chmod 700 ~/bin/do_jobs.sh"

for job in $(cat $JOBS_FILE); do
	echo "Starting on job: $job"
	for host in $(cat $HOSTS_FILE); do
		echo "Runing on host $host"
		ssh "$host" "do_job.sh '$job'"
	done
done
