from keras.layers import Dense
from keras.models import Sequential
from keras.optimizers import SGD
from keras.layers import Dropout
import numpy as np

data = np.loadtxt("GPS_Coordinates.csv",delimiter=",")
x = data[:,:-1]
y = data[:,-1]

# create model
model = Sequential()
model.add(Dense(3, input_dim=3, kernel_initializer='normal', activation='relu'))
model.add(Dense(1, kernel_initializer='normal', activation='sigmoid'))
# Compile model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])


idx = np.arange(len(y))
np.random.shuffle(idx)

div_factor = 0.8
train_idx = idx[:int(len(y)*div_factor)]
test_idx = idx[int(len(y)*div_factor):]
model.fit(x[train_idx,:],y[train_idx],epochs=2000,verbose=False);



acc = model.evaluate(x[test_idx,:],y[test_idx],verbose=False)[1]
print("Accuracy: {}".format(acc))
model.save('my_model.h5')  # creates a HDF5 file 'my_model.h5'
print(x
)
