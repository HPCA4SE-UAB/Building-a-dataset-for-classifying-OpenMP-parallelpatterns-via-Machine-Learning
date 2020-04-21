## How to execute the examples

In order to run the examples you need to install:

[`Python 3.6`](https://www.python.org/downloads/)


[`Jupyter`](https://jupyter.org/)

## File description

* Correlation analysis.ipynb :
	Script in jupyter notebook which uses Python to calculate the matrix correlation between different kernels and the patterns included in the pattern collection. This example compares Polybench kernels.

* Training ANN.ipynb:
	Script to train the ANN described in our work.

Sometimes github fails opening jupyter notebooks, if this problem appears, use nbviewer: https://nbviewer.jupyter.org/

## Database descriptopn

The databases were compressed in 7z format using 7za, it may be necessary to use git lfs to download the files instead of git due to file size.

* dataset_kernerls_corr.7z:
	Includes the dataset used in "Correlation analysis.ipynb". The columns in this dataset are: id (iteration id), size vector, threads, affinity, label and one column for each hardware performance counter
* train_ANN.7z:
    Includes the dataset used in "Training ANN.ipynb". The columns in this dataset are: label and one column for each hardware performance counter.
* test_ANN.7z:
    Includes the dataset used in "Training ANN.ipynb". The columns in this dataset are: label and one column for each hardware performance counter.


## LICENSE (GPL):

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.