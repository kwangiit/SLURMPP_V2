js=$1

salloc -N$js bash

echo "begin..."
./main

let "ur = ((90 / $js))"

for((i=1;i<=$ur;i++))
do
srun -l -o fsj$ur.out ./sleep
done
exit
