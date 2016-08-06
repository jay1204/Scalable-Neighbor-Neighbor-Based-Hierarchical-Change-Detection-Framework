import numpy as np
import matplotlib.pyplot as plt

def main():
  sample_size = [x for x in range(9,17)]
  
  two = [x*100 for x in [0.065,0.065,0.065,0.065,0.065,0.065,0.066,0.066]]
  three = [x*100 for x in [0.147,0.147,0.146,0.147,0.147,0.148,0.149,0.150]]
  four = [x*100 for x in [0.194,0.194,0.195,0.195,0.197,0.197,0.200,0.200]]
  five = [x*100 for x in [0.263,0.263,0.264,0.265,0.266,0.267,0.269,0.270]]
  six = [x*100 for x in [0.279,0.280,0.276,0.276,0.279,0.282,0.282,0.288]]
  seven = [x*100 for x in [0.298,0.300,0.295,0.294,0.297,0.299,0.300,0.305]]
  eight = [x*100 for x in [0.319,0.321,0.316,0.315,0.318,0.321,0.321,0.327]]

  plt.xlabel("The K in K-nearest Neighbor Classifier")
  plt.ylabel("Change Percentage|%")
  plt.axis([8,17,0,100])
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
