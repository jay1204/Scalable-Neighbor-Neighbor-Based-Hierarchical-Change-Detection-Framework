import numpy as np
import matplotlib.pyplot as plt

def main():
  sample_size = [1/500,1/1000,1/1500,1/2000,1/3000,1/4000,1/5000,1/6000,1/10000,1/15000,1/20000,1/40000]
  
  two = [41.4,43.06,46.09,46.71,52.15,53.03,60.56,66.04,68.4,98.74,125.76,247.14]
  #three = [x * 2948.2 for x in [1, 0.5,1/4,1/8,1/16,1/32,1/64]]

  plt.xlabel("The chunk size transfer from CPU to GPU")
  plt.ylabel("Running Time of Parallel Processing|second")
  plt.axis([0,1/500,0,260])
  two_line,= plt.plot(sample_size,two,label='Parallel',linewidth=2)
  #three_line, = plt.plot(sample_size,three,label='Serial Processing',linewidth=2)
  plt.setp(two_line,marker='o')
  #plt.setp(three_line,marker='o')
  plt.legend(bbox_to_anchor=(1.05,1),loc=1,borderaxespad=0.)
  plt.show()
  return

if __name__=="__main__":
    main()
