#!/bin/bash -l
#SBATCH -N 2
#SBATCH -t 00:10:00
#SBATCH -p debug
#SBATCH -L SCRATCH
#SBATCH --gres=craynetwork:2
#SBATCH -J dataspaces
#SBATCH -C haswell 

STAGING_METHOD=$1

DATASPACES_DIR=$SCRATCH/software/install/dataspaces
RUNCMD="srun -n"
#RUNCMD="mpirun -np"
#RUNCMD="aprun -n"
SERVER=$DATASPACES_DIR/bin/dataspaces_server

WRITEPROC=32
STAGINGPROC=2
READPROC=4
let "PROCALL=WRITEPROC+READPROC"
WRITEFREQ=0.0
READFREQ=0.0
LOCK=1
STEP=3

## Clean up
rm -f conf cred dataspaces.conf log.* jobid

NODES=($(scontrol show hostname $SLURM_NODELIST))

echo $SLURM_JOB_ID > jobid

## Create dataspaces configuration file
echo "## Config file for DataSpaces
ndim = 3
dims = 1024,1024,1024
max_versions = 3
max_readers = 1  
lock_type =" $LOCK > dataspaces.conf

## Run DataSpaces servers
echo "-- Start DataSpaces server "$SERVER" on $STAGINGPROC PEs, -s$STAGINGPROC -c$PROCALL"
srun -N 1 -n $STAGINGPROC -c 2 --nodelist=${NODES[0]} --gres=craynetwork:1 --mem 61400 $SERVER -s$STAGINGPROC -c$PROCALL &> log.server_$STAGING_METHOD &

## Give some time for the servers to load and startup
sleep 1s
while [ ! -f conf ]; do
    echo "-- File conf is not yet available from server. Sleep more"
    sleep 1s
done
sleep 3s  # wait server to fill up the conf file

## Export the main server config to the environment
while read line; do
    export set "${line}"
done < conf

echo "-- Dataspaces Servers initialize successfully"
echo "-- DataSpaces IDs: P2TNID = $P2TNID   P2TPID = $P2TPID"
echo "-- Staging Method: $STAGING_METHOD"

## Run reader application
echo "-- Start READER on $READPROC PEs"
srun -N 1 -n $READPROC -c 2 --nodelist=${NODES[0]} --gres=craynetwork:1 --mem 61400 ./reader $STAGING_METHOD $READPROC 3 2 2 1 512 512 1024 $STEP 2 $READFREQ &> log.reader_$STAGING_METHOD &

## Run writer application
echo "-- Start WRITER on $WRITEPROC PEs"
srun -N 1 -n $WRITEPROC -c 2 --nodelist=${NODES[1]} ./writer $STAGING_METHOD $WRITEPROC 3 4 4 2 256 256 512 $STEP 1 $WRITEFREQ &> log.writer_$STAGING_METHOD &

echo "-- Wait until all applications exit."
wait
echo "-- All applications exit."
