rm ctldHosts
for i in `seq 1 1`; do
echo node-${i}.sb.usrc.kodiak.nx >> ctldHosts
done

rm dHosts
for i in `seq 3 402`; do
echo node-${i}.sb.usrc.kodiak.nx >> dHosts
done

