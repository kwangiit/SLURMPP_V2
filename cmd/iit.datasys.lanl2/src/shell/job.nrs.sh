it=$1
for((i=1;i<=it;i++))
do
#./sbatch -n1024 -w "node-[3-514]" -o /users/xiaobing/lanl/log/job.$it.$i.out task.sh
./srun -N1 /bin/sleep 0 >  /users/xiaobing/lanl/log/job.$it.$i.out &
done
exit
