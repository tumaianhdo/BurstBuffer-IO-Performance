#!/bin/bash -l
file=analysis
jobid=$(cat jobid)
nservers=$(cat job.sl | grep STAGINGPROC= | cut -d'=' -f2)
nproducers=$(cat job.sl | grep WRITEPROC= | cut -d'=' -f2)
nconsumers=$(cat job.sl | grep READPROC= | cut -d'=' -f2)
nsteps=$(cat job.sl | grep STEP= | cut -d'=' -f2)
wfreq=$(cat job.sl | grep WRITEFREQ= | cut -d'=' -f2)
rfreq=$(cat job.sl | grep READFREQ= | cut -d'=' -f2)
lock=$(cat job.sl | grep LOCK= | cut -d'=' -f2)

logdir=result/new/${nservers}_${nproducers}_${nconsumers}_${nsteps}_${lock}_${wfreq}_${rfreq}
mkdir $logdir
cp log.* $logdir

echo "#Servers="$nservers >> $file
echo "#Producer="$nproducers >> $file
echo "#Consumers="$nconsumers >> $file
echo "#TimeStep="$nsteps >> $file
echo "#WriterFrequency="$wfreq >> $file
echo "#ReaderFrequency="$rfreq >> $file
echo "#LockType="$lock >> $file

cat dataspaces.conf | grep dims >> $file
echo " " >> $file
cat log.writer* | grep TS >> $file
cat log.reader* | grep TS >> $file 
#echo " " >> $file
#cat log.writer* | grep write_lock_acquire | grep 'ts 1' >> $file
#cat log.writer* | grep put_data | grep 'ts 1' >> $file
#cat log.writer* | grep write_lock_re | grep 'ts 1' >> $file
#echo " " >> $file
#cat log.reader* | grep read_lock_ac | grep 'ts 1' >> $file
#cat log.reader* | grep get_data | grep 'ts 1' >> $file
#cat log.reader* | grep read_lock_re | grep 'ts 1' >> $file

echo " " >> $file
sacct --jobs=$jobid --format=jobname,nodelist,elapsed,ntasks,alloccpus >> $file
echo "-----------------------------------------------" >> $file
