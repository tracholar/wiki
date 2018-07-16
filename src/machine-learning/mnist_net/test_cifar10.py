'''
Cifar-10 classification
Original dataset and info: https://www.cs.toronto.edu/~kriz/cifar.html for more information
See: https://www.bonaccorso.eu/2016/08/06/cifar-10-image-classification-with-keras-convnet/ for further information
'''

from __future__ import print_function

import numpy as np

from keras.callbacks import EarlyStopping, TensorBoard
from keras.datasets import cifar10
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Flatten
from keras.layers.convolutional import Conv2D
from keras.optimizers import Adam
from keras.layers.pooling import MaxPooling2D
from keras.utils import to_categorical
from utils import TrainValTensorBoard
from keras.layers import BatchNormalization
from keras.preprocessing.image import ImageDataGenerator
import itchat
import keras

# For reproducibility
np.random.seed(1000)


class wx_cb(keras.callbacks.Callback):
    def on_epoch_end(self, epoch, logs=None):
        itchat.send_msg('[epoch] ' + str(epoch) + ' ' + str(logs), toUserName='filehelper')

    def on_batch_end(self, batch, logs=None):
        if batch % 100 == 0:
            itchat.send_msg('[batch] ' + str(batch) + ' ' + str(logs), toUserName='filehelper')



if __name__ == '__main__':
    itchat.auto_login(hotReload=True, enableCmdQR=True)

    # Load the dataset
    (X_train, Y_train), (X_test, Y_test) = cifar10.load_data()

    # Create the model
    model = Sequential()

    model.add(Conv2D(32, kernel_size=(3, 1), activation='relu', input_shape=(32, 32, 3)))
    model.add(Conv2D(32, kernel_size=(1, 3), activation='relu'))

    model.add(BatchNormalization())
    model.add(Conv2D(32, kernel_size=(3, 1), activation='relu'))
    model.add(Conv2D(32, kernel_size=(1, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.5))

    model.add(Conv2D(64, kernel_size=(1, 3), activation='relu'))
    model.add(Conv2D(64, kernel_size=(3, 1), activation='relu'))
    model.add(BatchNormalization())
    model.add(Conv2D(64, kernel_size=(1, 3), activation='relu'))
    model.add(Conv2D(64, kernel_size=(3, 1), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Conv2D(128, kernel_size=(1, 3), activation='relu'))
    model.add(Conv2D(128, kernel_size=(3, 1), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.5))

    model.add(Flatten())
    model.add(Dense(1024, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(10, activation='softmax'))

    # Compile the model
    model.compile(loss='categorical_crossentropy',
                  optimizer=Adam(lr=0.001, decay=1e-6),
                  metrics=['accuracy'])

    # Train the model
    datagen = ImageDataGenerator(rotation_range=20, width_shift_range=0.2, height_shift_range=0.2)
    print(to_categorical(Y_train).shape)
    datagen.fit(X_train / 255.0)

    model.fit_generator(datagen.flow(X_train / 255.0, to_categorical(Y_train), batch_size=128),
              #batch_size=128,
              shuffle=True,
              epochs=100,
              validation_data=(X_test / 255.0, to_categorical(Y_test)),
              callbacks=[TrainValTensorBoard(log_dir='./logs/cnn_cifar5', histogram_freq=1),
                         wx_cb()]
              )

    model.fit(X_train / 255.0, to_categorical(Y_train),
              batch_size=128,
              shuffle=True,
              epochs=100,
              validation_data=(X_test / 255.0, to_categorical(Y_test)),
              callbacks=[TrainValTensorBoard(log_dir='./logs/cnn_cifar5', histogram_freq=1),
                         wx_cb()]
              )

    # Evaluate the model
    scores = model.evaluate(X_test / 255.0, to_categorical(Y_test))

    print('Loss: %.3f' % scores[0])
    print('Accuracy: %.3f' % scores[1])