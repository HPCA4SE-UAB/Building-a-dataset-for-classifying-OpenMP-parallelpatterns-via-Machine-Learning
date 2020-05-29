import os
import csv
import sys

#Get number of threads
print("Threads:  "+os.environ['OMP_NUM_THREADS'])
threads= os.environ['OMP_NUM_THREADS']


#Read csv
csv_files = [f for f in os.listdir('.') if (os.path.isfile(f) and f.endswith(".csv"))]
for f in csv_files:
  inp_name= f
  print(inp_name)
  inp = open(inp_name, 'r')
  print(threads+'-'+inp_name)
  out = open(threads+'-'+inp_name, 'w')
  reader = csv.reader(inp, delimiter=';')
  writer = csv.writer(out, delimiter=';')
  
  #Get position of '-' used to detect compilation flags and size
  kf = inp_name.find("-")
  kl = inp_name.rfind("-")
  new_string = inp_name[kl+1:]
  new_string1 = new_string.replace('.csv','')
  
  #If there are no compilation flags, insert NoFound
  if kf == kl :
    comp_opt = 'NF'
  else:
    comp_opt = inp_name[kf+1:kl]
  
  #Get size
  size_v = new_string1
  a = 0
  #In first row modify headers
  #In other rows, insert aditional data
  for row in reader:
    row1 = row[:-1]
    if a == 0:
      row1.insert(0,'id')
      row1.insert(1,'size vector')
      row1.insert(2,'comp opt')
      row1.insert(3,'threads')
    else: 
      row1.insert(0,a)
      row1.insert(1,size_v)
      row1.insert(2,comp_opt)
      row1.insert(3,threads)
    writer.writerow(row1)
    a=a+1
  #Close files
  inp.close()
  out.close()