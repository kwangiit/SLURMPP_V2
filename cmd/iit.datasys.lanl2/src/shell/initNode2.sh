echo "creating /users/xiaobing/lanl/log/"
mkdir /users/xiaobing/lanl/log/

echo ""
echo "creating links to srun, sinfo, squeue"
ln -s ../../../install/bin/srun
ln -s ../../../install/bin/sinfo
ln -s ../../../install/bin/squeue
ln -s ../../../install/bin/sbatch
ln -s ../../../install/bin/scancel
ln -s ../main
ln -s ../allone
ln -s ../sleep
ln -s ../proc_data


echo ""
echo "openssl -gen slurm.key slurm.cert"
openssl genrsa -out ../../../install/etc/slurm.key 1024
openssl rsa -in ../../../install/etc/slurm.key -pubout -out ../../../install/etc/slurm.cert


echo ""
echo "copying slurm.conf"
cp ../slurm.conf ../../../install/etc/

echo ""
echo "installing parallel-ssh"
sudo dpkg -i /users/xiaobing/Downloads/pssh_2.2.2-0ubuntu1_all.deb

echo ""
echo "rm .bashrc"
rm /users/xiaobing/.bashrc


echo ""
echo "export PATH"
echo "export PATH=/users/xiaobing/install/bin:/users/xiaobing/install/sbin:$PATH" >> /users/xiaobing/.bashrc

echo ""
echo "export LD_LIBRARY_PATH"
echo "export LD_LIBRARY_PATH=/users/xiaobing/install/lib:$LD_LIBRARY_PATH" >> /users/xiaobing/.bashrc

echo ""
echo "source .bashrc"
source  /users/xiaobing/.bashrc
