from keras.layers import Dense
from keras.models import Sequential
from keras.models import load_model
from keras.optimizers import SGD
from keras.layers import Dropout
import numpy as np

data = np.loadtxt("GPS_Coordinates.csv",delimiter=",")
#x = data[:,:-1]
#y = data[:,-1]

model = load_model('my_model.h5')

x_data = data[0:,:-1]
sample_num = 40
print(x_data[sample_num])

inp_data = x_data[sample_num][np.newaxis,:]
print(type(inp_data))
out_val = model.predict(inp_data)

print(out_val)

#np.put(out_val, [0], 2)
#print(out_val)


tex_data = np.array([-0.0644770677,-0.0127414433,1])
inp_data = tex_data[np.newaxis,:]
print(tex_data)
print(type(tex_data))
out_val = model.predict(inp_data)

print(out_val)


#Insert class id externally
id = 2
tex_data = np.array([-0.0644770677,-0.0127414433,id])


x_data = data[0:,:-1]
sample_num = 11
print(x_data[sample_num])

new_data = x_data[sample_num]
np.put(new_data, [2], id)


inp_data = new_data[np.newaxis,:]
print(type(inp_data))
out_val = model.predict(inp_data)

print(out_val)


