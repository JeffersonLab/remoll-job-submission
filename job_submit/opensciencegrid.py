import sys
import os
import re
import csv

configfile = sys.argv[1]

p={}    # dictionary of parameter values

with open(configfile) as csvfile:
  reader=csv.reader(csvfile, delimiter=',', quotechar='|')
  for row in reader:
    if row[2].strip()=="string":
      p[row[0]]=row[1].strip()
    else:
      p[row[0]]=float(row[1])

condor_sim_file=open(p["condor_sim_file"], "w")
condor_sim_file.write("test")               
condor_sim_file.close()
