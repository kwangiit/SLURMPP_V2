echo ""
echo "rm /tmp/log on all compute nodes"
parallel-ssh -t 0 -A --hosts=dHosts -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null -o /users/xiaobing/lanl/boot/dout -e /users/xiaobing/lanl/boot/derr 'rm -f -r /tmp/log/'

