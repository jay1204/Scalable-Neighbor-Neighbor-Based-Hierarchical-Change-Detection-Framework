import numpy as np
import matplotlib.pyplot as plt

def main():
  sample_size = [x*100 for x in range(1,11)]
  
  two = [x*100 for x in [0.1126, 0.0768,0.1131 ,0.123 ,0.0426, 0.0706, 0.1045, 0.1009, 0.0775, 0.0847]]
  three = [x*100 for x in [0.1884, 0.0829, 0.117, 0.1278, 0.1229, 0.0708, 0.1081, 0.1042, 0.0778, 0.1039]]
  four = [x*100 for x in [0.2184, 0.1342, 0.1936, 0.2483, 0.2018, 0.1488, 0.2061, 0.22, 0.1507, 0.1851 ]]
  five = [x*100 for x in [0.2416, 0.2068, 0.2757, 0.2675, 0.202, 0.1531, 0.2497, 0.2426, 0.2125, 0.1853, ]]
  six = [x*100 for x in [0.2972, 0.2395, 0.3438, 0.3309, 0.3427, 0.1867, 0.3159, 0.307, 0.2318, 0.2508]]
  seven = [x*100 for x in [0.3642, 0.3241, 0.3443, 0.3766, 0.3449, 0.2176, 0.3161, 0.3819, 0.266, 0.3001]]
  eight = [x*100 for x in [0.405, 0.364, 0.367, 0.4057, 0.363288, 0.2639, 0.3289, 0.3991, 0.2948, 0.3607]]

  plt.xlabel("Sampling Size")
  plt.ylabel("Change Percentage|%")
  plt.axis([0,1100,0,100])
  two_line, = plt.plot(sample_size,two,label='Two Groups',linewidth=2)
  three_line, = plt.plot(sample_size,three,label='Three Groups',linewidth=2)
  four_line, = plt.plot(sample_size,four,label='Four Groups',linewidth=2)
  five_line, = plt.plot(sample_size,five,label='Five Groups',linewidth=2)
  six_line, = plt.plot(sample_size,six,label='Six Groups',linewidth=2)
  seven_line, = plt.plot(sample_size,seven,label='Seven Groups',linewidth=2)
  eight_line, = plt.plot(sample_size,eight,label='Eight Groups',linewidth=2)
  plt.setp(two_line,marker='o')
  plt.setp(three_line,marker='o')
  plt.setp(four_line,marker='o')
  plt.setp(five_line,marker='o')
  plt.setp(six_line,marker='o')
  plt.setp(seven_line,marker='o')
  plt.setp(eight_line,marker='o')
  plt.legend(bbox_to_anchor=(1.05,1),loc=1,borderaxespad=0.)
  plt.show()
  return

if __name__=="__main__":
    main()
