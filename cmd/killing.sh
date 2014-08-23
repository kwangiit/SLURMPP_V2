echo "kill all the controllers"
n=$1
parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=host "killall -9 slurmctld &"

sleep 5

echo "kill all the zht servers"
parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=host "killall -9 zhtserver &"

echo "kill all the slurmds"
for i in `seq 1 $n`; do
        parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=slurmnode_${i} "killall -9 slurmd &"
        parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=slurmnode_${i} "killall -9 slurmstepd &"
done
