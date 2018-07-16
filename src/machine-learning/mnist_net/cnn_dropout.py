#coding:utf-8
# LR model

import tensorflow as tf
import numpy as np

from tensorflow.examples.tutorials.mnist import input_data

class CNN(object):
    def __init__(self):
        self.x = tf.placeholder(dtype=tf.float32, shape=(None, 28, 28, 1), name='x')
        self.y = tf.placeholder(dtype=tf.float32, shape=(None, 10), name='y')
        self.W1 = tf.Variable(initial_value=np.random.randn(5, 5, 1, 50)/5/np.sqrt(2), dtype=tf.float32, name='W1')
        self.b1 = tf.Variable(initial_value=np.random.randn(1, 50)/5/np.sqrt(2), dtype=tf.float32, name='b1')

        self.W2 = tf.Variable(initial_value=np.random.randn(5, 5, 50, 32)/25/2, dtype=tf.float32, name='W2')
        self.b2 = tf.Variable(initial_value=np.random.randn(1, 32)/25/2, dtype=tf.float32, name='b2')

        z = tf.nn.conv2d(tf.nn.dropout(self.x, 0.8), self.W1, strides=[1, 1, 1, 1], padding='SAME') + self.b1
        a = tf.nn.relu(tf.nn.max_pool(z, ksize=[1, 4, 4, 1], strides=[1, 3, 3, 1], padding='SAME'))
        #a = tf.nn.dropout(a, 0.8, name='dropout1')

        z2 = tf.nn.conv2d(a, self.W2, strides=[1, 1, 1, 1], padding='SAME') + self.b2
        a2 = tf.nn.relu(tf.nn.max_pool(z2, ksize=[1, 7, 7, 1], strides=[1,3,3, 1], padding='SAME'))
        #a2 = tf.nn.dropout(a2, 0.8, name='dropout2')

        self.W3 = tf.Variable(initial_value=np.random.randn(4*4*32,10)/16/2, dtype=tf.float32, name='W3')
        self.b3 = tf.Variable(initial_value=np.random.randn(1, 10)/16/2, dtype=tf.float32, name='b3')

        y_ = tf.matmul(tf.reshape(a2, (-1, 4*4*32)), self.W3) + self.b3

        cross_entropy = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(labels=self.y, logits=y_))
        reg = tf.reduce_sum(self.W1 ** 2) + tf.reduce_sum(self.W2 ** 2)
        self.mu = tf.placeholder(dtype=tf.float32)
        self.loss = cross_entropy + self.mu * reg
        self.train_op = tf.train.AdamOptimizer(learning_rate=0.01).minimize(self.loss)

        correct = tf.equal(tf.argmax(self.y,1), tf.argmax(y_,1))
        self.accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))

        tf.summary.scalar('acc', self.accuracy)
        tf.summary.scalar('loss', self.loss)
        tf.summary.scalar('mean_W1', tf.reduce_mean(tf.abs(self.W1)))
        tf.summary.scalar('mean_W2', tf.reduce_mean(tf.abs(self.W2)))
        tf.summary.scalar('mean_W3', tf.reduce_mean(tf.abs(self.W3)))

        self.sess = tf.Session()

        self.merged_summary_op = tf.summary.merge_all()
        self.summary_writer = tf.summary.FileWriter('./logs/cnn_dropout2_adam', self.sess.graph)
        self.sess.run(tf.global_variables_initializer())

    def train(self):
        mnist = input_data.read_data_sets("../MNIST_data/", one_hot=True)

        for i in range(1000):
            x, y = mnist.train.next_batch(128)
            loss, _ = self.sess.run([self.loss, self.train_op], feed_dict={self.x : x.reshape(-1, 28, 28, 1), self.y: y, self.mu : 0.0001})

            if (i+1) % 10 == 0:
                summary_str, acc = self.sess.run([self.merged_summary_op, self.accuracy], feed_dict={self.x: mnist.test.images.reshape(-1, 28, 28, 1), self.y: mnist.test.labels, self.mu: 0.01})
                print '\ri=',i, 'LOSS=', loss, 'ACC=', acc,

                self.summary_writer.add_summary(summary_str, i)





if __name__ == '__main__':
    cnn = CNN()
    cnn.train()
