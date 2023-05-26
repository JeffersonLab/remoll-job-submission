# Setting up the environment

To set up ROOT and GEANT4 environment:

	source /site/12gev_phys/softenv.csh 2.5

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
