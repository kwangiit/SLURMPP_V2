it=$1
rm -f job.srs.$it.ld
for((i=1;i<=$(($it/2));i++))
do
n=$((RANDOM%50))
s=$(($n+1+50))
echo $s >> job.srs.$it.ld
#echo $s
done
exit
