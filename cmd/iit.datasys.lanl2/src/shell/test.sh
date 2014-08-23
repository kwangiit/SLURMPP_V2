js=$1

./main

let "ur = ((90 / $js))"

for((i=1;i<=$ur;i++))
do
echo $i > fsj$i.out
done

