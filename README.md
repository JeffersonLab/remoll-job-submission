# Setting up environment.

To setup a environment with geant4/10.06 and root/6.20.04:
	
	source ~/projects/rrg-jmammei/REMOLL/environment/cedar_env_2.0.sh

Note that geant4 visualisation does not work on beluga with this environment. This should be the default environment to run array jobs. If you need visualisation to work on geometry design, use the older environment:

	source ~/projects/rrg-jmammei/REMOLL/environment/cedar_env.sh

# File management

Work by creating subdirectories under the ~/projects/rrg-jmammei folder. Make sure that the group ownership of the folder is set to rrg-jmammei. 

If you already have data in your /project directory with the wrong group ownership, you can use the find to display those files:

	lfs find ~/projects/*/ -group $USER

Next, change group ownership from $USER to the project group, for example:

	chown -h -R $USER:rrg-jmammei -- ~/projects/rrg-jmammei/$USER/

Then, set the setgid bit on all directories (for more information, see Group ID) to ensure that newly created files will inherit the directory's group membership, for example:

	lfs find ~/projects/rrg-jmammei/$USER -type d -print0 | xargs -0 chmod g+s

# Backing up data

It is possible to backup data in the nearline tape storage. Back up your data here if you don't plan to access them for a long time.

	cd /nearline/rrg-jmammei/$USER 
	dar -c <output_directory_name> -R <full path of directory containing source directory> -g <source_directory_name> -s 100G 

# Running interactive jobs

For commands that take long time to execute,  you can use interactive jobs. Useful when combining large number of ROOT files together using hadd. 

You can start an interactive session on a compute node with salloc. In the following example we request two tasks, which corresponds to two CPU cores, for an hour:

	salloc --time=1:0:0 --ntasks=2 --account=def-someuser
	salloc: Granted job allocation 1234567
	...             # do some work
	exit            # terminate the allocation
	salloc: Relinquishing job allocation 1234567

It is also possible to run graphical programs interactively on a compute node by adding the --x11 flag to your salloc command. In order for this to work, you must first connect to the cluster with X11 forwarding enabled.

# Checking existing jobs in the slurm queue

Maximum 1000 jobs can be simultaneously run on beluga per user. To see the list of existing jobs in your queue:

	squeue -u $USER




