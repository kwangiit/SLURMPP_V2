output=/users/kwangiit/output

rm -rf $output

parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=host "sudo cp /users/kwangiit/hpdc14/cmd/sysctl.conf /etc/; sudo sysctl -p >> $output"

parallel-ssh -t 0 -o /tmp/ -p 250 -O StrictHostKeyChecking=no -O UserKnownHostsFile=/dev/null --hosts=host "sudo cp /users/kwangiit/hpdc14/cmd/limits.conf /etc/security/"

sleep 3
