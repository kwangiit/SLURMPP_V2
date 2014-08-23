it=$1
for((i=1;i<=it;i++))
do
./srun -N900 -n1800 /bin/sleep 0 >  /users/xiaobing/lanl/log/job.$it.$i.out &
done
exit
