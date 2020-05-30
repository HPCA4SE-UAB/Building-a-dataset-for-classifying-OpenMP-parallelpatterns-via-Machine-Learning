## How to execute the examples

In order to run the examples you need to install:

[`Python 3.6`](https://www.python.org/downloads/)


[`Jupyter`](https://jupyter.org/)


[`MATE`](https://github.com/HPCA4SE-UAB/MATE)

## Directory structure

 ```bash
├───MATE_tunlet #Tunlet for MATE, should be copied into MATE's src folder before compiling: [MATE_dir]/src/Analyzer
└───polybench #Modified Polybench benchmark with OpenMP parallel versions. Documentation about polybench can be found inside polybench.pdf
	│ 		  # and original repository https://web.cse.ohio-state.edu/~pouchet.2/software/polybench/
    ├───jobs  # Includes scripts to execute MATE using SLURM and scripts to read intermediary results to produce database
	│   │	  # Absolute paths in scripts should be modified
    │   └───results #The final database is generated inside this folder
    ├───linear-algebra #Linear-algebra kernels, read Polybench documentation
    │   ├───blas
    │   │   ├───gemm
    │   │   ├───gemver
    │   │   ├───gesummv
    │   │   ├───symm
    │   │   ├───syr2k
    │   │   ├───syrk
    │   │   └───trmm
    │   └───kernels 
    │       ├───2mm
    │       ├───3mm
    │       ├───atax
    │       ├───doitgen
    │       └───mvt
    ├───stencils #stencils kernels, read Polybench documentation
    │   ├───adi
    │   ├───fdtd-2d
    │   ├───heat-3d
    │   ├───jacobi-1d
    │   └───jacobi-2d
    └───utilities #Polybench common files for all benchmarks, read Polybench documentation
 ```
## Jupyter's notebooks description

* Correlation analysis.ipynb :
	Script in jupyter notebook which uses Python to calculate the matrix correlation between different kernels and the patterns included in the pattern collection. This example compares Polybench kernels.

* Training ANN.ipynb:
	Script to train the ANN described in our work.

Sometimes github fails opening jupyter notebooks, if this problem appears, use nbviewer: https://nbviewer.jupyter.org/

## Instructions

To reproduce the results in another hardware, these are the steps that the user should follow:

1. Install PAPI. Refer to: http://icl.utk.edu/papi/software/

2. Download MATE. Refer to: https://github.com/HPCA4SE-UAB/MATE

	- Copy Tunlets files into MATE's src/Analyzer

	- Install MATE's dependencies and MATE. Refer to: https://github.com/HPCA4SE-UAB/MATE/tree/master/docs

3. Modify all .ini files in benchmarks subdirectories with the paths chosen in MATE's installation.

4. Modify SLURM's scripts inside polybench/jobs directory to fit queue's and hardware's parameters. Also, POLY_PATH should be modified with the full path of the polybench directory. 

5. Send jobs to queue with  sbatch job-poly-AC.sub; sbatch job-poly-Analyzer.sub;

6. If the jobs ended without errors, modify the paths in polybench/jobs/fuse_results.sh (variable POLY_PATH).

7. Execute fuse_results.sh

8. When it ends, a csv fill all the execution data should have been generated.

9. Use Correlation analysis.ipynb to find correlations between the different kernels and remove the highly correlated ones.

	- Noncorrelated kernels should be saved in train.csv and removed kernels in test.csv.

10. Traning ANN.ipynb can be used now to train an ANN to classify kernels.


Notes:

	1. If additional fields were generated or added to the database, they should be removed to work without modifying the jupyter notebook provided.
	
		- Fields can be checked with name_dataset.info()
		
		- The fields can be removed with: del name_dataset['name_field'] after pd.read_csv
		
		
	2. The process of data normalization can be seen in Training ANN.ipynb, under Import validation dataset in the second executable cell.



## Database creation

In order to create the database, we have used MATE to obtain the hardware performance counter values. It is also possible to use PTF, TAU2, or use PAPI directly to measure them.

We provide scripts for our case using MATE.

To execute and obtain the performance data, scripts polybench/jobs/job-poly-AC.sub and polybench/jobs/job-poly-Analyzer.sub are provided. They should be modified according to the slurm queue and hardware used.

Also, the .ini files found in each benchmark subdirectory should be modified according to MATE's installation paths and log's paths.

To generate the database for correlation, the fuse_results.sh script under polybench/jobs should be used.

Train_ANN and test_ANN datasets are generated using correlation analysis as explained in the paper.


## Database description

The databases were compressed in 7z format using 7za, it may be necessary to use git lfs to download the files instead of git due to file size.

* dataset_kernerls_corr.7z:
	Includes the dataset used in "Correlation analysis.ipynb". The columns in this dataset are: id (iteration id), size vector, threads, affinity, label and one column for each hardware performance counter
* train_ANN.7z:
    Includes the dataset used in "Training ANN.ipynb". The columns in this dataset are: label and one column for each hardware performance counter.
* test_ANN.7z:
    Includes the dataset used in "Training ANN.ipynb". The columns in this dataset are: label and one column for each hardware performance counter.

	
Databases were normalized dividing all the hardware counters in the each row by the value of PAPI_REF_CYC in each row.



[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3865286.svg)](https://doi.org/10.5281/zenodo.3865286)



## LICENSE (GPL):

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
