start=$1
end=$2
i=1
while read line
do           
ctrlmachine=$(echo "$line" | awk -F' ' '{print $2}')
ctrladdr=$(echo "$line" | awk -F' ' '{print $1}')
echo "generatiing slurm_$i.conf"
./gen_conf -c slurm_ke.conf -t slv_hosts -s slurm_$i.conf -a $start -d $end -m $ctrlmachine -r $ctrladdr
#echo "./gen_conf -c slurm_ke.conf -t slv_hosts -s slurm_$i.conf -a $start -d $end -m $ctrlmachine -r $ctrladdr"
let "i=i+1"
done <ctl_hosts 


#./gen_conf -c slurm2.conf -t slv_hosts -s slurm.conf -a 2 -d 102 -m node-1 -r ko0097
#./gen_conf -c slurm_ke.conf -t slv_hosts -s slurm_2.conf -a 3 -d 508 -m ko0823 -r node-2

