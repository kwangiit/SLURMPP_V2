echo ""
echo "mkdir /tmp/log on all compute nodes"
parallel-ssh -t 0 -A --hosts=dHosts -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null -o /users/xiaobing/lanl/boot/dout -e /users/xiaobing/lanl/boot/derr 'mkdir /tmp/log/'
