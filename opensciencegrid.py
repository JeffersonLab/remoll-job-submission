import sys
import os
import re


configfile = sys.argv[1]

condor_sim_file=open(p["condor_sim_file"], "w")
condor_sim_file.write("test")               
condor_sim_file.close()
