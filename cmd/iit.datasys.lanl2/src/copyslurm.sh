#!/bin/bash
n=$1 
p=$2 
for i in `seq 1 ${n}`
do
	cp slurm_${i}.conf ~/slurm_${i}/slurm.${i}.${n}.${p}.conf
done
