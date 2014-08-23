#parallel-ssh -v -t 0 -p 1 -A -O StrictHostKeyChecking=no --hosts=dHosts -o /tmp/psshout -e /tmp/pssherr '/bin/sleep 0' 
parallel-ssh -t 0 -A --hosts=ctldHosts -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null -o /users/xiaobing/lanl/boot/ctldout  -e /users/xiaobing/lanl/boot/ctlderr/ 'sudo /users/xiaobing/install/sbin/slurmctld -c -L /users/xiaobing/lanl/boot/ctld.log'

#parallel-ssh -t 0 -A --hosts=dHosts -o /users/xiaobing/lanl/boot/dlog 'bash -c echo "hello2"'
parallel-ssh -t 0 -A --hosts=dHosts -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null -o /users/xiaobing/lanl/boot/dout -e /users/xiaobing/lanl/boot/derr 'sudo /users/xiaobing/install/sbin/slurmd -c -L /users/xiaobing/lanl/boot/d.log'


