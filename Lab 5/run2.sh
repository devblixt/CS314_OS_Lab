cp schedule.c /usr/src/minix/servers/sched/;
cp -R /workloads_lab5 ../home/byte-unixbench-mod/UnixBench/workload_mix/;
cd /usr/src/;
make build MKUPDATE=yes >log.txt 2>log.txt