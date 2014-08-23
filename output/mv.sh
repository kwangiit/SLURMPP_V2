#!/bin/bash

num_controller=$1
part_size=$2
num_time=$3

mkdir job_time_$1.$2.$3
mv job_time/srun_job_time* job_time_$1.$2.$3

mkdir zht_msg_$1.$2.$3
mv zht_msg/msg_record_* zht_msg_$1.$2.$3
