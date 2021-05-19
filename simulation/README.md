 cd /w/work/halla/moller12gev/<username>
 mkdir <study_dir>
• cd <study_dir>
• mkdir <job_dir>
• cd <job_dir>
• cp /volatile/halla/moller12gev/rahmans/REMOLL_TEST/*.py .
• git checkout jlab
• git clone https://github.com/JeffersonLab/remoll.git
• cd remoll
• git checkout develop
• mkdir build
• cd build
• source /site/12gev_phys/softenv.csh 2.4
• source /site/12gev_phys/2.4/Linux_CentOS7.7.1908-gcc9.2.0/geant4/4.10.06.p02MT/bin/geant4.csh
• cmake -DCMAKE_BUILD_TYPE=Release ../
• make -j 4
• cp libremoll.so ../
• cd <job_dir>
• ./macro.py -h
• ./macro.py -a halla -s . -v remoll -j /volatile/halla/moller12gev/<username>/<job
name>/jsub -t /volatile/halla/moller12gev/<username>/<jobname>/tmp -o
/volatile/halla/moller12gev/<username>/<job name>/scratch -g <generator>
-d 28 --bhd 28 -r 1-10 -n 1000000 --time 06:00:00
