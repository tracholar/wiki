#coding:utf-8
# LR model

import tensorflow as tf
from keras.layers import Dense, Conv2D, InputLayer, MaxPool2D, Flatten, Dropout
from keras.models import Sequential
from keras.losses import categorical_crossentropy
from keras.optimizers import Adam
from keras.callbacks import TensorBoard
from utils import TrainValTensorBoard
import numpy as np

from tensorflow.examples.tutorials.mnist import input_data

class CNN(object):
    def __init__(self):
        self.model = Sequential([
            Conv2D(50, (5, 5), input_shape=(28, 28, 1), padding='same', activation='relu'),
            MaxPool2D(pool_size=(4, 4), strides=(3, 3), padding='same'),

            Conv2D(32, (5, 5), padding='same', activation='relu' ),
            MaxPool2D(pool_size=(7, 7), strides=(3, 3), padding='same'),

            Flatten(),
            Dropout(0.5),
            Dense(64, activation='relu'),
            Dense(10, activation='softmax')
        ])

        self.model.compile(loss=categorical_crossentropy, optimizer=Adam(), metrics=['accuracy'])

    def train(self):
        mnist = input_data.read_data_sets("../MNIST_data/", one_hot=True)

        x_test, y_test = mnist.test.images.reshape(-1, 28, 28, 1), mnist.test.labels
        self.model.fit(mnist.train.images.reshape(-1, 28, 28, 1), mnist.train.labels,
                       batch_size=128,
                       epochs=20,
                       verbose=1,
                       validation_data=(x_test, y_test),
                       callbacks=[TrainValTensorBoard(log_dir='./logs/cnn3', histogram_freq=1, write_grads=True)])

        score = self.model.evaluate(x_test, y_test, verbose=0)
        print('Loss', score[0], 'acc', score[1])





if __name__ == '__main__':
    cnn = CNN()
    cnn.train()
