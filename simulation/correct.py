#!/bin/env python3

import ROOT as R
import glob
import os
import sys
import argparse

parser = argparse.ArgumentParser(description="Delete corrupted files and print number of good files.")

parser.add_argument("-c", dest="conf", action="store", required=True, help="choose config")
parser.add_argument("-g", dest="gen", action="store", required=True, help="choose gen")
args=parser.parse_args()

file_list= glob.glob(args.conf+"/"+args.gen+"_*.root")

corrupt= 0
good= 0
for file_name in file_list:
  f= R.TFile(file_name)
  good=good+1
  if f.IsZombie() or  f.GetSize()<1000: 
    print(f.GetName()+" is zombie \n")
    corrupt=corrupt+1
    os.system("rm "+file_name)
    good=good-1
  if f.TestBit(R.TFile.kRecovered):
    print(f.GetName()+" is corrupt but recovered\n")
    corrupt=corrupt+1
    os.system("rm "+file_name)
    good=good-1

print("Corrupt: "+str(corrupt))
print("Good: "+str(good))


