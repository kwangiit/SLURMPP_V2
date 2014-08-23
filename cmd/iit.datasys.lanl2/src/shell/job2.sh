it=$1
sl=$2
for((i=1;i<=$it;i++))
do
./srun -N$sl -n$((2*$sl)) /bin/sleep 0 >  /users/xiaobing/lanl/log/job.$sl.$i.out &
#echo $sl $((2*$sl))
done
exit
