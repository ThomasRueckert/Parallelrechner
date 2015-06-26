./Uebung8/helloworldoffload
./Uebung8/helloworldoffloadPhi 
export LD_LIBRARY_PATH=/opt/intel/lib/mic
./Uebung8/helloworldoffloadPhi 
./Uebung8/helloworldoffload
./Uebung8/helloworld
./Uebung8/helloworldPhi 
./Uebung8/helloworldoffloadPhi 
./Uebung8/helloworldoffloadPhi 
source /opt/intel/composer_xe_2015/bin/compilervars.sh intel64
./Uebung8/helloworldoffloadPhi 
./Uebung8/helloworldoffload
./Uebung8/helloworldoffload
./Uebung8/helloworldPhi 
export LD_LIBRARY_PATH=/opt/intel/lib/mic
./Uebung8/helloworldPhi 
module avail
module list
module load intel/15.0.2 
module list
ll
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Hochleistungsrechner/Uebung8/ .
ll
cd Uebung8/
ll
clear
ll
icc helloworld -o helloworld
icc helloworld -o helloworld -mmic
icc helloworld.c -o helloworld -mmic
icc helloworld.c -o helloworld -mmic -fopenmp
ll
icc helloworld.c -o helloworld -fopenmp
./helloworld 
icc helloworldoffload.c -o helloworldoffload -fopenmp
vim helloworldoffload.c
icc helloworldoffload.c -o helloworldoffload -fopenmp
./helloworldoffload 
vim helloworldoffload.c
icc helloworldoffload.c -o helloworldoffload -fopenmp
./helloworldoffload 
vim helloworldoffload.c
micnativeloadex helloworldoffload
micnativeloadex helloworld
icc helloworldoffload.c -o helloworldoffloadPhi -fopenmp -mmic
icc helloworld.c -o helloworldPhi -fopenmp -mmic
micnativeloadex helloworldPhi 
SINK_LD_LIBRARY_PATH=/opt/intel/composer_xe_2015/lib/mic/ micnativeloadex helloworldPhi 
SINK_LD_LIBRARY_PATH=/opt/intel/composer_xe_2015/lib/mic/ micnativeloadex helloworldoffload
vim helloworldoffload.c
icc helloworldoffload.c -o helloworldoffloadPhi -fopenmp -mmic
ssh mic0
rm helloworldoffloadPhi 
icc helloworldoffload.c -o helloworldoffloadPhi -fopenmp -mmic
ll
ssh mic0
micnativeloadex helloworldoffload
micnativeloadex helloworldoffloadPhi 
micnativeloadex helloworld
ssh mic0
micnativeloadex helloworldoffload
micnativeloadex helloworldoffloadPhi 
micnativeloadex helloworld
ll
icc helloworld.c -o helloworld 
icc helloworld.c -o helloworld -openmp
./helloworld
icc helloworld.c -o helloworldPhi -openmp -mmic
micnativeloadex helloworldPhi 
SINK_LD_LIBRARY_PATH=/opt/intel/composer_xe_2015/lib/mic/ micnativeloadex helloworldPhi 
ssh mic0
vim helloworld
vim helloworld.c
icc helloworld.c -o helloworldPhi -openmp -mmic
vim helloworld.c
icc helloworld.c -o helloworldPhi -openmp -mmic
vim helloworld.c
icc helloworld.c -o helloworldPhi -openmp -mmic
vim helloworld.c
icc helloworldoffload.c -o helloworldoffloadPhi -openmp -mmic
vim helloworldoffload.c
icc helloworldoffload.c -o helloworldoffloadPhi -openmp -mmic
icc helloworldoffload.c -o helloworldoffloadPhi -openmp
./helloworldoffloadPhi 
cat helloworldoffloadPhi 
cat helloworldoffloadPhi.c
clear
cat helloworldoffloadPhi.c
cat helloworldoffload.c
clear
micnativeloadex helloworldoffloadPhi 
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Hochleistungsrechner/Uebung8/ue8.c ue8.c
icc icc -std=c99 -openmp ue8.c -o ue8
icc -std=c99 -openmp ue8.c -o ue8
./ue8
vim ue8.c
icc -std=c99 -openmp ue8.c -o ue8
./ue8
time ./ue8
vim ue8.c
icc -std=c99 -openmp ue8.c -o ue8
time ./ue8
vim ue8.c
./ue8
vim ue8.c
./ue8
while [ true ]; do ./ue8; done
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/client.c .
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/server.c .
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/Makefile .
ll
make
vim server.c
make
vim Makefile 
man rsocket
vim Makefile 
make
vim Makefile 
make
vim server.c
make
man rsocket
vim server.c
make
./server 
ip addr
./server 
vim client.c
./server 
make
./server 
man rsocket
man inet_pton
vim client.c
man inet_pton
./server 
./clientSock 127.0.0.1
man rsocket
./client 192.168.2.60/24
./client 192.168.2.60
./client 127.0.0.1
 192.168.4.255
./client 192.168.4.255
./client 192.168.3.60/24
./client 192.168.3.60
./client 192.168.3.60/24
./client 127.0.0.1
./client 192.168.2.60/24
./client 192.168.2.60
cat client.c
vim server.c
make
./server 
vim server
vim server.c 
vim client.c
make
./server 
./clie
vim client.c
./client 192.168.2.60
./client 192.168.2.60/24
./client 192.168.2.60:24
./client 192.168.2.60
ll
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/clientSock.c .
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/serverSock.c .
cp -R /afs/tu-chemnitz.de/home/urz/r/rutho/Parallelrechner/Uebung8/server.c serverSock.c
cp Makefile MakefileRsock
vim Makefile
vim serverSock.c 
vim clientSock.c 
vim Makefile
make
ll
./serverSock 
