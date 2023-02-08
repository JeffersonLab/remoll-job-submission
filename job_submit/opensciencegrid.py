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
    elif row[2].strip()=="list":
      p[row[0]]=row[1]
    else:
      p[row[0]]=float(row[1])

condor_sim_file=open(p["condor_submit_file"], "w")
for i in p["remollana_type"]:
  condor_sim_file.write(i)               
condor_sim_file.close()
