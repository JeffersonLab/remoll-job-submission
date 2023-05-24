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
 cd <job_dir>
 ./macro.py -h
 ./macro.py -a rrg-jmammei -s ~/projects/rrg-jmammei/<username>/<simulation_directory>
 -v remoll -j <output_folder>/<job_name>/jsub -t <output_folder>/<jobname>/tmp -o
<output_folder>/<jobname>/scratch -g <generator> -d 28 38  39 40 41 42 43 44 45 --bhd 28 38 39 40 41 42 43 44 45 -r 1-100 -n 75000 --time 10:00:00
'''
