import numpy as np

data = np.genfromtxt('iris.csv', delimiter=',')
last_column = data[:, -1]
data = data[:, :-1]
data = np.concatenate((last_column[:, np.newaxis], data), axis=1)

np.savetxt('output.txt', data, delimiter=' ')