import sys

def main():
  matchfile = open("hierarchical_clustering_labels_2_8_Euclidean_172.txt","rb")
  matchlines = matchfile.readlines()
  lookTable = []
  for line in matchlines:
    myTable = map(int,line.split(" "))
    lookTable.append(myTable)

  recordfile = open("recordfile.txt",'w')

  leftstr = "k="
  for i in range(9,17):
    recordfile.write("leftstr"+str(i)+'\n')
    diff = [0 for x in range(7)]
    filename1 = leftstr+str(i)+"/whole_col_p_out1.txt"
    filename2 = leftstr+str(i)+"/whole_col_p_out2.txt"
    f1 = open(filename1,"rb")
    f2 = open(filename2,"rb")
    outputfilenamepart = leftstr+str(i)+ "/whole_possible_change_map_k_"+str(i)+"_"
    outputMatrix = [[0 for x in range(9600)] for x in range(7)]
    possChangefilename = leftstr+str(i)+"/whole_possible_change_map.txt"
    possChangefile = open(possChangefilename,'w')
    

    outputfiles = [open(outputfilenamepart+"%d_clusters.txt" % m, "w") for m in range(2, 9)]
    while 1:
      line1 = f1.readline()
      line2 = f2.readline()

      possChangeMap = [0 for x in range(9600)]

      if line1 and line2:
        list1 = map(int,line1.split(" ")[:-1])
        list2 = map(int,line2.split(" ")[:-1])
        if len(list1)!=len(list2):
          print "the column in the file not match: " + filename1
        else:
          for k in range(0,len(list1)):
            for n in range(7):
              #print lookTable[list1[k]-1][n],lookTable[list2[k]-1][n]
              if lookTable[list1[k]-1][n] == lookTable[list2[k]-1][n]:
                outputMatrix[n][k] = 0
              else:
                outputMatrix[n][k] = 1
                diff[n]+=1
                possChangeMap[k]+=1
          for l in range(7):
            outputstr = ' '.join(map(str,outputMatrix[l]))+'\n'
            #print outputstr
            outputfiles[l].write(outputstr)
          outputPoss = ' '.join(map(str,possChangeMap))+'\n'
          possChangefile.write(outputPoss)
      else:
         break

    for u in range(7):
      outputfiles[u].close()
      outputstring = str(u+2) + ":"+ str(float(diff[u])/(9600*4800)) + " change points: " + str(diff[u]) + '\n'
      recordfile.write(outputstring)
    f1.close()
    f2.close()
    possChangefile.close()

    print leftstr + str(k)
  recordfile.close()
  matchfile.close()

if __name__ == '__main__':
  main()
        
       
      

              
