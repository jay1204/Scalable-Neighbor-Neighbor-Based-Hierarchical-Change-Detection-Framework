import numpy as np
import matplotlib.pyplot as plt

def main():
  sample_size = [x*100 for x in range(1,11)]
  
  two = [x for x in [326.7,636.54,950.73,1271.46,1583.64,1938.85,2233.11,2587.92,2873.68,3244.69]]

  plt.xlabel("Sampling Size")
  plt.ylabel("Running Time|second")
  plt.axis([0,1100,0,3500])
  two_line,= plt.plot(sample_size,two,label='Two Groups',linewidth=2)
  plt.setp(two_line,marker='o')
  #plt.legend(bbox_to_anchor=(1.05,1),loc=1,borderaxespad=0.)
  plt.show()
  return

if __name__=="__main__":
    main()
