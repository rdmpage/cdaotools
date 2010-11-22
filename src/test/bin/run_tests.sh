#/bin/bash
export PATH="~/bin:$PATH"
export HOSTS_FILE="$1"
export JOBS_FILE="$2"

#scp do_jobs.sh daedalus.cs.nmsu.edu:~/bin/
#ssh daedalus.cs.nmsu.edu "chmod 700 ~/bin/do_jobs.sh"

for host in $(cat $HOSTS_FILE); do
	ssh "$host" "do_jobs.sh '$JOBS_FILE'"
done