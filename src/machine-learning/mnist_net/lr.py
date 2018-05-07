#coding:utf-8
# LR model

import tensorflow as tf
import numpy as np

from tensorflow.examples.tutorials.mnist import input_data

class LR(object):
    def __init__(self):
        self.x = tf.placeholder(dtype=tf.float32, shape=(None, 784))
        self.y = tf.placeholder(dtype=tf.float32, shape=(None, 10))
        self.W = tf.Variable(initial_value=np.random.rand(784, 10), dtype=tf.float32)
        self.b = tf.Variable(initial_value=np.random.rand(1, 10), dtype=tf.float32)

        y_ = tf.matmul(self.x, self.W) + self.b

        cross_entropy = tf.reduce_mean( - tf.reduce_sum(self.y * tf.log(tf.nn.softmax(y_)), axis=1))
        reg = tf.reduce_sum(self.W ** 2)
        self.mu = tf.placeholder(dtype=tf.float32)
        self.loss = cross_entropy + self.mu * reg
        self.train_op = tf.train.RMSPropOptimizer(learning_rate=0.01).minimize(self.loss)

        correct = tf.equal(tf.argmax(self.y,1), tf.argmax(y_,1))
        self.accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))

        tf.summary.scalar('acc', self.accuracy)
        tf.summary.scalar('loss', self.loss)
        tf.summary.scalar('mean(W)', tf.reduce_mean(tf.abs(self.W)))
        self.sess = tf.Session()

        self.merged_summary_op = tf.summary.merge_all()
        self.summary_writer = tf.summary.FileWriter('./logs/lr_rmsp', self.sess.graph)
        self.sess.run(tf.global_variables_initializer())

    def train(self):
        mnist = input_data.read_data_sets("../MNIST_data/", one_hot=True)



        for i in range(30000):
            x, y = mnist.train.next_batch(128)
            loss, _ = self.sess.run([self.loss, self.train_op], feed_dict={self.x : x, self.y: y, self.mu : 0.01})

            if (i+1) % 100 == 0:
                summary_str, acc = self.sess.run([self.merged_summary_op, self.accuracy], feed_dict={self.x: mnist.test.images, self.y: mnist.test.labels, self.mu: 0.01})
                print '\rLOSS=', loss, 'ACC=', acc,

                self.summary_writer.add_summary(summary_str, i)





if __name__ == '__main__':
    lr = LR()
    lr.train()
