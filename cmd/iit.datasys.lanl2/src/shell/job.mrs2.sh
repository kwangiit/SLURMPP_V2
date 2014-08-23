it=$1
let "i=0"
for s in $(cat load/job.mrs.$it.ld)
do
let "i=$i+1"
./srun -N$s /bin/sleep 0 >  /users/xiaobing/lanl/log/job.$it.$i.out &
#echo $s
done
exit

