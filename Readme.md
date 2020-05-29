## How to execute the examples

In order to run the examples you need to install:

[`Python 3.6`](https://www.python.org/downloads/)


[`Jupyter`](https://jupyter.org/)


[`MATE`](https://github.com/HPCA4SE-UAB/MATE)

## File description

* Correlation analysis.ipynb :
	Script in jupyter notebook which uses Python to calculate the matrix correlation between different kernels and the patterns included in the pattern collection. This example compares Polybench kernels.

* Training ANN.ipynb:
	Script to train the ANN described in our work.

Sometimes github fails opening jupyter notebooks, if this problem appears, use nbviewer: https://nbviewer.jupyter.org/

## Database creation

In order to create the database, we have used MATE to obtain the hardware performance counter values. It is also possible to use PTF, TAU2, or use PAPI directly to measure them.

We provide scripts for our case using MATE.

To execute and obtain the performance data, scripts polybench/jobs/job-poly-AC.sub and polybench/jobs/job-poly-Analyzer.sub are provided. They should be modified according to the slurm queue and hardware used.

Also, the .ini files should be modified according to MATE's installation paths and log's paths.

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

## LICENSE (GPL):

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.