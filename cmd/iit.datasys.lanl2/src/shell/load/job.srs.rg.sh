it=$1
rm -f job.srs.$it.ld
for((i=1;i<=it;i++))
do
let "r=(($it/2))"
n=$((RANDOM%$r))
s=$(($n+1+$r))
#./srun -N$s /bin/sleep 0 >  /users/xiaobing/lanl/log/job.$it.$i.out &
echo $s >> job.srs.$it.ld
done
exit
