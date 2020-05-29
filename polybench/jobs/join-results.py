import glob
import pandas as pd
import numpy as np

#Look for all csv files
result_files = sorted(glob.glob("*.csv"))
print ( result_files )

first = 1

#For each csv
for file_r in result_files:
	print( file_r )
	#Read csv as a dataframe
	file_df = pd.read_csv(file_r, sep = ";", index_col=['id','size vector','comp opt','label','threads'], dtype=str)
	print( file_df.index.get_level_values('label') )
	#For the first file, create a dataframe with all the input data
	if first:
		df_all_results = file_df
		first = 0
	#For the remaining files, concatenate them at the end
	else:
		#df_all_results= df_all_results.append(file_df)
		df_all_results= pd.concat([df_all_results,file_df])
	#Remove intermediare dataframes
	del file_df

	
print( df_all_results.index.get_level_values('label') )
df_all_results.info()
#Save dataframe
df_all_results.to_csv("polybench_results.csv", sep = ";", chunksize=1000000 )