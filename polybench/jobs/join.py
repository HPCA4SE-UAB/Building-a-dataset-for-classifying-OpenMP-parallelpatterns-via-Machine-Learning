import glob
import pandas as pd
import numpy as np
import sys, getopt

#Obtain the affinity of the files 
try:
	opts, args = getopt.getopt(sys.argv[1:],"cs",["close","spread"])
except getopt.GetoptError:
	print( 'Script should be called as: join.py -[c|s|close|spread] ' )
	print( str(err) )
	print( sys.argv[1:] )
	sys.exit(2)
	  
for opt, arg in opts:
	if opt in ("-c", "--close"):
		affinity = "close"
	elif opt in ("-s", "--spread"):
		affinity = "spread"
	print( "Affinity -- "+affinity )

	
	

#Create empty dataframe
dfall = pd.DataFrame()
print(dfall)

#Get the directories for the different Tunlets
directories = sorted(glob.glob("Tunlet*/"), key=str.lower)
print (directories)

a = 0
tunlet_num = 0
#For each Tunlet
for direct_i in directories:
  print (direct_i)
  tunlet_num = tunlet_num + 1
  #Set the time field name to include the Tunlet number of that execution
  str_time = "timeT_" + str(tunlet_num)
  files = glob.glob(direct_i+"*.csv")
  b = 0
  df_direct = pd.DataFrame()
  #For each execution with different number of threads
  for file_i in files:
    print (file_i )
	#Read csv
    df = pd.read_csv(file_i, sep = ";", index_col=['id','size vector','comp opt','label','threads'])
    print (df.isnull().sum().sum())
    print (dfall.isnull().sum().sum())
    print (df.shape[0])
	#Check there are no empty fields, if there are empty fields, the execution failed at some point
    if dfall.isnull().values.any():
      print("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
      exit(1)
    if df.isnull().values.any():
      print("ERROR1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
      exit(1)
    #Rename time columns
    df.rename(columns = {"time": str_time}, inplace = True )
	
	#For the first csv, declare the first dataframe as the Tunlet dataframe
    if b==0:
      df_direct = df
      b = 1
    #Add the csvs to the Tunlet dataframe
	else:
      df_direct = df_direct.append(df)
  #For the first Tunlet, declare the Tunlet dataframe as the global dataframe
  if a==0:
      dfall = df_direct
      a=1
  #For the remaining Tunlets, merge the Tunlet dataframe into the global dataframe
  else:
    print('Hello')
    print(df_direct)
    print(dfall)
    dfall = dfall.merge(df_direct, right_on=['id','size vector','comp opt','label','threads'], left_on=['id','size vector','comp opt','label','threads'], how='outer')
    print(dfall)
 
	
 
#Save file to disk
dfall.to_csv(affinity+'-data.csv', sep = ";" )

#Check if there are no empty fields
print(dfall.isnull().values.any())
if dfall.isnull().values.any():
    exit(1)
