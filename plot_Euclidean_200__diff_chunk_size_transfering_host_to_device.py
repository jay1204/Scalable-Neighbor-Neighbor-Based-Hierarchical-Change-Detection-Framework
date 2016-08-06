import numpy as np
import matplotlib.pyplot as plt

def main():
  sample_size = [x for x in [1, 0.5,1/4,1/8,1/16,1/32,1/64]]
  
  two = [39.9, 20.61,10.76,5.37,2.73,1.38,0.69]
  three = [x * 2948.2 for x in [1, 0.5,1/4,1/8,1/16,1/32,1/64]]

  plt.xlabel("The size of the input")
  plt.ylabel("Running Time of Parallel Processing|second")
  plt.axis([0,1,0,3200])
  two_line,= plt.plot(sample_size,two,label='Parallel',linewidth=2)
  three_line, = plt.plot(sample_size,three,label='Serial Processing',linewidth=2)
  plt.setp(two_line,marker='o')
  plt.setp(three_line,marker='o')
  plt.legend(bbox_to_anchor=(1.05,1),loc=1,borderaxespad=0.)
  plt.show()
  return

if __name__=="__main__":
    main()
