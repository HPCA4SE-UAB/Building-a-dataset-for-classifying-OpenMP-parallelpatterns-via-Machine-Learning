#!/bin/bash
# exit when any command fails
set -e

SCRIPT_DIR=/home/caos/jalcaraz/polybench-c-4.2.1-beta-OPENMP/jobs/
RESULTS_DIR=/home/caos/jalcaraz/polybench-c-4.2.1-beta-OPENMP/jobs/results

#Join close and round robin(spread) affinity's files into one
fuse_close_spread()
{
	python3 ${SCRIPT_DIR}join-close-spread.py --close close/close-data.csv --spread spread/spread-data.csv --benchname $1
  if [ $? -eq 0 ]
  then
    echo "Successfull"
  else
    echo "Error"
    exit 1
  fi
}

#Join all files for all Tunlets for one affinity
fuse_files_affinity()
{
	python3 ${SCRIPT_DIR}join.py $1
}


#Method to fuse data from different Tunlets into one csv for each affinity
#At the end, the different csv files will be joined into one with both affinities
check_fuse_close_spread()
{
	CURRENT_DIR=`pwd`; 
	bench=`basename $CURRENT_DIR`; 
	
	#Check that both directories for both affinities exist
	echo "Checking dirs...";
	if [[ -d  close && -d  spread ]]
	then
		echo -e "\e[92mDirectories found, processing files...\e[39m";
		echo "$Fusing bechmark: ${bench} (inside ${CURRENT_DIR} )...";
		#Fuse files for close affinity
		cd close
			fuse_files_affinity --close;
		cd ..
		#Fuse files for round robin(spread) affinity
		cd spread 
			fuse_files_affinity --spread;
		cd ..
		
		echo "fuse_close_spread";
		#Fuse files of both affinities into one
		fuse_close_spread $bench;
		file_name=fused-${bench}.csv
		
		#Copy file to results' directory
		echo $file_name
		echo $RESULTS_DIR
		echo ${RESULTS_DIR}/${file_name}
		cp -v ${file_name} ${RESULTS_DIR}/${file_name}
		echo "$Fused bechmark: ${bench} (inside ${CURRENT_DIR} )...";	
		
	else
		echo -e "\e[31mERROR!!!!!!!!!!!!!! directories not found!!!!!\e[39m";
		exit 1
	fi
	
}


#Check for subdirectories where benchmarks are localted
# and fuse the data for the two affinities
explore_dirs() 
{
  CURRENT_DIR_E=`pwd`; 
	for dir in ${CURRENT_DIR_E}/*
	do
	  cd $dir
	  #execute method check_fuse_close_spread
	  check_fuse_close_spread;
    cd ..
	done
}


#Move to the main directory of the benchmarks
cd /home/caos/jalcaraz/polybench-c-4.2.1-beta-OPENMP/

###{ 


#Move to the linear-algebra benchmarks<
cd linear-algebra
	#Move to blas
	cd blas
		explore_dirs
	cd ..
	
	#Move to kernels
	cd kernels
		explore_dirs
	cd ..

cd ..

#Move to stencil benchmarks
cd stencils
	explore_dirs
cd .. 




#Fuse the different intermediary files into one csv with all the benchmarks
cd $RESULTS_DIR
	python3 ${SCRIPT_DIR}join-results.py



###} > fuse.log 2>&1 