#!/usr/bin/env python

import sys
import os
import subprocess
import math
import time
import argparse

parser = argparse.ArgumentParser(description="Submit array jobs to GREX.")
parser.add_argument("-a", dest="account", action="store", required=True, help="Enter the Compute Canada account. You can use the allocated account rrg-jmammei or the default account def-<supervisor username>. For test simulations, preferably use the def account or short stat with rrg.")  
parser.add_argument("-s", dest="src", action="store", required=False, default="~/projects/rrg-jmammei/REMOLL/remoll_version", help="source folder where simulation directory exists")
parser.add_argument("-v", dest="version", action="store", required=False, default="real_shield", help= "choose the version of simulation to use. current options are develop, kryp_shield, real_shield and real_shield_v1 [most optimized real shielding]. ")
parser.add_argument("-j", dest="jsub_dir", action="store", required=True, help="choose directory to write the slurm submission scripts")
parser.add_argument("-t", dest="tmp_dir", action="store", required=True, help="choose directory to write the slurm output logs")
parser.add_argument("-o", dest="out_dir", action="store", required=True, help="choose where to write the output root files")
parser.add_argument("-g", dest="gen", action= "store", required=False, default="moller",  help="choose generator to use. Options are moller, elastic, inelastic, beam, etc.")
parser.add_argument("-d", dest="det_list", action= "store", required=False, nargs='+', default=[28], help="provide list of sensitive detectors separated by space. Example: 28 29. By default, all detectors detect low energy neutrals and secondaries. ")
parser.add_argument("--bhd", dest="bhd_list", action= "store", required=False,nargs='+', default=[28], help="provide list of boundary hit detectors. By default only detector 28 is boundary hit detector")
parser.add_argument("-r", dest="run_range", action = "store", required=False, default="1", help="provide run range. Example: \"2-5\"")
parser.add_argument("-n", dest="n_events", action= "store", required=False, default=1000, help= "provide number of events per job in the array")
parser.add_argument("--time", dest="time", action= "store", required= False, default= "00:25:00", help= "provide the estimated run time. Ex: \"00:25:00\". Usually it is 10 minutes for 1000 moller events.")
parser.add_argument("--G4", dest="geant", action= "store", required=False, default="4",  help="choose geant4 version to use. 4 or 6.")
parser.add_argument("--umap", dest="umap", action = "store", required=False, default="upstreamJLAB_v2", help="provide the map name for upstream magnet. Ex: upstreamJLAB_v2.txt")
parser.add_argument("--dmap", dest="dmap", action = "store", required=False, default="segmentedJLAB_v2", help="provide the map name for downstream magnet. Ex: segmentedJLAB_v2.txt")
parser.add_argument("--targ", dest="targ", action = "store", required=False, default="LH2", help="provide the target name. Use USAl and DSAl for  aluminum generators.")
parser.add_argument("--pion", dest="pion", action = "store", required=False, default="pi-", help="provide the pion name.")
parser.add_argument("--interpol", dest="interpol", action="store", required=True, help="provide interpolation.")

args=parser.parse_args()

args.src=os.path.realpath(args.src)

if not os.path.exists(args.jsub_dir):
        os.system("mkdir -p "+args.jsub_dir)
if not os.path.exists(args.tmp_dir):
        os.system("mkdir -p "+args.tmp_dir)
if not os.path.exists(args.out_dir):
        os.system("mkdir -p "+args.out_dir)
args.tmp_dir=os.path.realpath(args.tmp_dir)
args.jsub_dir=os.path.realpath(args.jsub_dir)
out=os.path.realpath(args.out_dir)
		
jsubf=open(args.jsub_dir+"/"+args.gen+".sh", "w")
jsubf.write("#!/bin/bash\n")
jsubf.write("#SBATCH --account="+args.account+"\n")
jsubf.write("#SBATCH --job-name=remoll\n")
jsubf.write("#SBATCH --time="+args.time+" \n")
jsubf.write("#SBATCH --nodes=1\n")
jsubf.write("#SBATCH --ntasks=1\n")
jsubf.write("#SBATCH --cpus-per-task=5\n")
jsubf.write("#SBATCH --mem=10G\n")
jsubf.write("#SBATCH --output="+args.tmp_dir+"/"+args.gen+"_%A_%a.out\n")
#jsubf.write("#SBATCH --error="+args.tmp_dir+"/"+args.gen+"_%A_%a.err\n")
if (args.geant=="6"):
   jsubf.write("source ~/projects/rrg-jmammei/REMOLL/environment/cedar_env_2.0.sh \n")
else:
   jsubf.write("source ~/projects/rrg-jmammei/REMOLL/environment/cedar_env.sh \n")

macro="$TMPDIR/"+args.gen+"_${SLURM_JOBID}_${SLURM_ARRAY_TASK_ID}.mac"
jsubf.write("touch "+macro+"\n")
jsubf.write("echo /remoll/setgeofile geometry/mollerMother.gdml >>"+macro+"\n")
jsubf.write("echo /remoll/physlist/register QGSP_BERT_HP >>"+macro+"\n")
jsubf.write("echo /remoll/physlist/parallel/enable >>"+macro+"\n") 
jsubf.write("echo /remoll/parallel/setfile geometry/mollerParallel.gdml >>"+macro+"\n")
jsubf.write("echo /run/numberOfThreads 5 >>"+macro+"\n")
jsubf.write("echo /run/initialize >>"+macro+"\n")
jsubf.write("echo /remoll/addfield map_directory/"+args.dmap+" >>"+macro+"\n")
jsubf.write("echo /remoll/addfield map_directory/"+args.umap+" >>"+macro+"\n")	
jsubf.write("echo /remoll/field/interpolation map_directory/"+args.dmap+" "+args.interpol+" >>"+macro+"\n")
jsubf.write("echo /remoll/field/interpolation map_directory/"+args.umap+" "+args.interpol+" >>"+macro+"\n")
jsubf.write("echo /remoll/evgen/set "+args.gen+" >>"+macro+"\n")
if args.gen=="beam":
    jsubf.write("echo /remoll/evgen/beam/origin 0 0 -7.5 m >>"+macro+"\n")
    jsubf.write("echo /remoll/evgen/beam/rasx 5 mm >>"+macro+"\n")
    jsubf.write("echo /remoll/evgen/beam/rasy 5 mm >>"+macro+"\n")
    jsubf.write("echo /remoll/evgen/beam/corrx 0.065 >>"+macro+"\n")
    jsubf.write("echo /remoll/evgen/beam/corry 0.065 >>"+macro+"\n")
    jsubf.write("echo /remoll/evgen/beam/rasrefz -4.5 m >>"+macro+"\n")
else:
    jsubf.write("echo /remoll/oldras false >>"+macro+"\n")

jsubf.write("echo /remoll/geometry/absolute_position targetLadder \(0,0,0\) >>"+macro+"\n")
jsubf.write("echo /remoll/target/mother LH2 >>"+macro+"\n")
if (args.gen=="inelasticAl" or args.gen=="elasticAl" or args.gen=="quasielasticAl"):
    jsubf.write("echo /remoll/target/volume "+args.targ+"  >>"+macro+"\n")

if (args.gen=="pion"):
    jsubf.write("echo /remoll/evgen/pion/settype "+args.pion+" >>"+macro+"\n")

jsubf.write("echo /remoll/beamene 11 GeV >>"+macro+"\n")
jsubf.write("echo /remoll/beamcurr 85 microampere >>"+macro+"\n")
jsubf.write("echo /remoll/SD/disable_all >>"+macro+"\n")
for det in args.det_list:
    jsubf.write("echo /remoll/SD/enable "+str(det)+" >>"+macro+"\n")
    jsubf.write("echo /remoll/SD/detect lowenergyneutral "+str(det)+" >>"+macro+"\n")
    jsubf.write("echo /remoll/SD/detect secondaries "+str(det)+" >>"+macro+"\n")
    if (det in args.bhd_list):
       jsubf.write("echo /remoll/SD/detect boundaryhits "+str(det)+" >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/volume logicUSTracker >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/volume logicDSTracker >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/volume logicWasher_12 >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/volume lefthut_Det_inside_logic >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/volume righthut_Det_inside_logic >>"+macro+"\n")
#for i in range(1,8):
#    for j in range(1,4):
#        jsubf.write("echo /remoll/kryptonite/volume logic_outer_E"+str(j)+"_"+str(i)+" >>"+macro+"\n")
#for i in range(1,8):
#    jsubf.write("echo /remoll/kryptonite/volume logic_outer_E_"+str(i)+" >>"+macro+"\n")
#    jsubf.write("echo /remoll/kryptonite/volume logic_inner_E_"+str(i)+" >>"+macro+"\n")
#    jsubf.write("echo /remoll/kryptonite/volume logic_C_"+str(i)+" >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logicUScoll_1 >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logicUScollfin_1 >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logicUScollunion_1 >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logicUScollunion_2 >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logic_shieldUSbelly >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume shield_US_beampipe_1_logic >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logic_shield_top >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume logic_shield_bot >>"+macro+"\n")
#jsubf.write("echo /remoll/kryptonite/volume shield_DS_beampipe_logic >>"+macro+"\n")
jsubf.write("echo /remoll/kryptonite/enable >>"+macro+"\n")
jsubf.write("echo /remoll/filename "+out+"/"+args.gen+"_${SLURM_JOBID}_${SLURM_ARRAY_TASK_ID}.root >>"+macro+"\n")
jsubf.write("echo /run/beamOn "+str(args.n_events)+" >>"+macro+"\n")  
jsubf.write("cat "+macro+"\n")

jsubf.write("cp -r "+args.src+"/"+args.version+" $TMPDIR\n")
jsubf.write("cd $TMPDIR/"+args.version+" \n")
jsubf.write("echo \"Current working directory is `pwd`\"\n")
jsubf.write("./build/remoll "+macro+"\n")
jsubf.write("echo \"Program remoll finished with exit code $? at: `date`\"\n")
jsubf.close()
	        
                
subprocess.call("sbatch --array="+args.run_range+" "+args.jsub_dir+"/"+args.gen+".sh",shell=True)
		
		

		
	
	
