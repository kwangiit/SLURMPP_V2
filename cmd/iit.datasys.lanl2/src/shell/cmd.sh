scp -r /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/src/ xzhou40@216.47.152.142:/home/xzhou40/lanl
scp -r xzhou40@216.47.152.142:/home/xzhou40/lanl/src /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/2-ben




scp -r /home/xiaobing/Downloads xiaobing@kodiak.nmc-probe.org:/users/xiaobing/



scp -r /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/src xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl
scp -r /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl2/src xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl


scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/src/shell/task.*.out /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-kodiak
scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/src/shell/task.*.ben /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-kodiak

scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/log/log.tar /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-ko2
scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/log2/log2.tar /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-ko2
scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/logp/logp.tar /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-ko2

scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/log /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-ko2

/home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl/ben/ben-ko2/log-6878



scp -r xiaobing@kodiak.nmc-probe.org:/users/xiaobing/lanl/log/job.4.*.out /home/xiaobing/IIT-study/research/workspace/iit.datasys/iit.datasys.lanl2/ben/ben-ko



 ./gen_conf -c slurm2.conf -t host_file -s slurm.conf




./sbatch -n1024 -w "node-[3-514]" -o job.16364.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.8182.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.4096.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.2048.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.1024.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.512.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.256.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.128.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.64.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.32.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.16.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.8.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.4.out task.sh
./sbatch -n1024 -w "node-[3-514]" -o job.2.out task.sh



./sbatch -n1024 -w "node-[3-514]" -o /users/xiaobing/lanl/logp/task.512.1.out task2.sh 512
./sbatch -n512 -w "node-[3-258]" -o /users/xiaobing/lanl/logp/task.256.1.out task2.sh 256
./sbatch -n256 -w "node-[3-130]" -o /users/xiaobing/lanl/logp/task.128.1.out task2.sh 128
./sbatch -n128 -w "node-[3-66]" -o /users/xiaobing/lanl/logp/task.64.1.out task2.sh 64
./sbatch -n64 -w "node-[3-34]" -o /users/xiaobing/lanl/logp/task.32.1.out task2.sh 32
./sbatch -n32 -w "node-[3-18]" -o /users/xiaobing/lanl/logp/task.16.1.out task2.sh 16
./sbatch -n16 -w "node-[3-10]" -o /users/xiaobing/lanl/logp/task.8.1.out task2.sh 8
./sbatch -n8 -w "node-[3-6]" -o /users/xiaobing/lanl/logp/task.4.1.out task2.sh 4
./sbatch -n4 -w "node-[3-4]" -o /users/xiaobing/lanl/logp/task.2.1.out task2.sh 2



sbatch -n10 -w "node-[1-10]" -o task10.out task.sh
sbatch -n20 -w "node-[1-20]" -o task20.out task.sh
sbatch -n30 -w "node-[1-30]" -o task30.out task.sh
sbatch -n40 -w "node-[1-40]" -o task40.out task.sh
sbatch -n50 -w "node-[1-50]" -o task50.out task.sh
sbatch -n60 -w "node-[1-60]" -o task60.out task.sh
sbatch -n70 -w "node-[1-70]" -o task70.out task.sh
sbatch -n80 -w "node-[1-80]" -o task80.out task.sh
sbatch -n90 -w "node-[1-90]" -o task90.out task.sh


900

salloc -N10 bash


salloc -N20 bash
salloc -N30 bash
salloc -N40 bash
salloc -N50 bash
salloc -N60 bash
salloc -N70 bash
salloc -N80 bash
salloc -N90 bash


i=1

begin=$1
end=$2
for (( i = $begin; i <= $end; i++)) 
do
	echo "woking on node-$i"
	ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no tony@node-$i sudo /home/tony/SYSTEM_MAINTAIN/MAINTAIN_boot.local
done

squeue:
 Job state, compact form:  PD  (pending),  R  (running),  CA
                  (cancelled),  CG  (completing), CD (completed), F (failed),
                  TO (timeout), and NF (node failure).   See  the  JOB  STATE
                  CODES section below for more information.


sinfo:
 -t <states> , --states=<states>
              List nodes only having the given state(s).  Multiple states may
              be comma separated and  the  comparison  is  case  insensitive.
              Possible  values  include (case insensitive): ALLOC, ALLOCATED,
              COMP, COMPLETING, DOWN, DRAIN, DRAINED, DRNG,  DRAINING,  IDLE,
              UNK,  and UNKNOWN.  By default nodes in the specified state are
              reported whether they are responding or not.   The  --dead  and
              --responding  options  may  be  used  to filtering nodes by the
              responding flag.

