import glob
import pandas as pd
import numpy as np
import argparse

#List to parse MATE labels into benchmark labels
list_events = ["Copy_E","Scale_E","Add_E","Triad_E","Reduction_E","2PStencil_E","2D4PStencil_E","Gather_E","Scatter_E","Stride2_E","Stride4_E","Stride16_E","Stride64_E","Rows_E","Test_E"]


close = ''
spread = ''
benchname = ''

#Input arguments
parser = argparse.ArgumentParser(prog='ARGUMENTS', usage='%(prog)s [options]')
#Path to the close affinity file
parser.add_argument("--close", type=str, help="data close csv")
#Path to the round robin affinity file
parser.add_argument("--spread", type=str, help="data spread csv")
#Benchmark name
parser.add_argument("--benchname", type=str, help="benchmark name")

#Read and parse arguments
args = parser.parse_args()
close = args.close
spread = args.spread
benchname = args.benchname



print( close )
print( spread )
print( benchname )

#Check input arguments
if((close == '' ) or (spread == '') or (benchname == '')):
	print("Error: missing parameters")
	print( 'Script should be called as: join.py -[c|close] file -[s|spread] file -[b|benchname] name' )
	sys.exit(2)




#Create dataframe for close affinity and add affinity to the dataframe
dfclose = pd.read_csv(close, sep = ";" )
dfclose['affinity'] = "close"

#Create dataframe for round robin affinity and add affinity to the dataframe
dfspread = pd.read_csv(spread, sep = ";" )
dfspread['affinity'] = "spread"


#Join dataframes
dfall =  dfclose.append(dfspread)
print( set(dfall['label']) )

#Print labels
print( len( set(dfall['label'])) )

#Parse MATE¡s labels into benchmarks labels
if len( set(dfall['label'])) > 1 :
  for label in set(dfall['label']):
    print( label, list_events.index(label) )
    label_bench = benchname + "_" + str(list_events.index(label))
    dfall['label'].replace(label,label_bench,inplace=True)
else:
  dfall['label'] = benchname



#Save dataframe
dfall.to_csv('fused-'+benchname+'.csv', sep = ";", index = False)
