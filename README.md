# Setting up the environment

To set up ROOT/6.20.04 and Geant4/10.6.p02:

	source /site/12gev_phys/softenv.csh 2.4
	source /site/12gev_phys/2.4/Linux_CentOS7.7.1908-gcc9.2.0/geant4/4.10.06.p02MT/bin/geant4.csh

The above environment is the default environment for running simulations. 

The ROOT in the above environment doesn't support combining of files over a certain size using hadd. Use the following version for that purpose:

	source /apps/root/6.18.04/setroots_CUE




# Running interactive jobs on ifarm

From an ifarm machine, it is possible to create a shell on a farm node like so:

	ifarm> salloc -p ifarm
	salloc: Granted job allocation 5956045

	ifarm> srun --pty bash

	bash-4.2$ echo "This is running on host `hostname`"
	This is running on host farm180301

	bash-4.2$ exit
	exit

	ifarm> exit
	exit

	salloc: Relinquishing job allocation 5956045
 
Note that there are two steps: First is getting an allocation on the slurm ifarm partition. The second is attaching to the session on the farm node. Once the job is attached to farm node all computation happens there. The shell session is a conduit for the interaction.

# Submitting parallel Remoll jobs on ifarm

Look at the instructions in the simulation and analysis folders

# Checking job status

Maximum 1000 jobs can be run simultaneously on ifarm. Check your queue status before submitting more jobs.

	squeue -u $USER
