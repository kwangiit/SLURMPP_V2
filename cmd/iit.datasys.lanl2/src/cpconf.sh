n=$1
p=$2

for i in `seq 1 $n`; do
	scp -o 'StrictHostKeyChecking no' -o 'UserKnownHostsFile /dev/null' slurm_$i.conf kwangiit@kodiak.nmc-probe.org:./slurm_$i/slurm.$i.$n.$p.conf
done

