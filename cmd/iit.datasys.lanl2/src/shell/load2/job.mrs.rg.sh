it=$1
rm -f job.mrs.$it.ld
for((i=1;i<=$(($it/2));i++))
do
n=$((RANDOM%50))
s=$(($n+1))
echo $s >> job.mrs.$it.ld
#echo $s
done
exit
