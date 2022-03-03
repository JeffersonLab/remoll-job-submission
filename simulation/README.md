 # Instructions
 ```
 git clone https://github.com/JeffersonLab/remoll-job-submission.git
 git checkout beluga
 cd remoll-job-submission
 cd simulation
 git clone https://github.com/JeffersonLab/remoll.git
 cd remoll
 git checkout develop
 mkdir build
 cd build
 source /lustre03/project/rrg-jmammei/REMOLL/environment/cedar_env_2.0.sh
 cmake -DCMAKE_BUILD_TYPE=Release ../
 make -j 4
 cp libremoll.so ../
 cd <job_dir>
 ./macro.py -h
 ./macro.py -a halla -s . -v remoll -j /volatile/halla/moller12gev/<username>/<job
name>/jsub -t /volatile/halla/moller12gev/<username>/<jobname>/tmp -o
/volatile/halla/moller12gev/<username>/<job name>/scratch -g <generator>
-d 28 --bhd 28 -r 1-10 -n 1000000 --time 06:00:00
'''
