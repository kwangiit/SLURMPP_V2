 echo ""
 echo "###########gen_conf doing"
 sleep 1
 cd .. 
 ./gen_conf -c slurm2.conf -t host_file -s slurm.conf

 
 echo ""
 echo "###########cleanNFS doing"
 sleep 1
 cd shell
 bash cleanNFS.sh


 #bash cleanAllNodes.sh

 
 echo ""
 echo "###########initNode2 doing"
 sleep 1
 bash initNode2.sh


 #bash initAllNodes.sh

 echo ""
 echo "###########genSlurmHosts doing"
 sleep 1
 bash genSlurmHosts.sh


 echo ""
 echo "###########startAllSlurm doing"
 sleep 1
 bash startAllSlurm.sh
