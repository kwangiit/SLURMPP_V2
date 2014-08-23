#!/bin/sh
it=$1
./main 1> /users/xiaobing/lanl/logp/task.$it.0.out

#SBATCH --time=0
./srun -l ./allone
