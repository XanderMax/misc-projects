import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'  # or any {'0', '1', '2'}
import argparse
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

def main(pretrained_model_path):
    mnist = tf.keras.datasets.mnist
    (x_train,y_train), (x_test,y_test) = mnist.load_data()

    x_train = tf.keras.utils.normalize(x_train,axis=1)
    x_test = tf.keras.utils.normalize(x_test,axis=1)

    model = train_model(x_train, y_train, x_test, y_test)

    model.save(pretrained_model_path)

def train_model(x_train, y_train, x_test, y_test):
    model = tf.keras.models.Sequential()

    model.add(tf.keras.layers.Flatten(input_shape=(28, 28)))

    model.add(tf.keras.layers.Dense(128,activation=tf.nn.relu))
    model.add(tf.keras.layers.Dense(128,activation=tf.nn.relu))
    model.add(tf.keras.layers.Dense(10,activation=tf.nn.softmax))

    model.compile(optimizer='adam',
                loss='sparse_categorical_crossentropy',
                metrics=['accuracy']
                )
    model.fit(x_train,y_train,epochs=3)
    val_loss,val_acc = model.evaluate(x_test,y_test)
    print("loss-> ",val_loss,"\nacc-> ",val_acc)

    return model

def parse_arguments():
    parser = argparse.ArgumentParser(description='Image Classifier')
    parser.add_argument('--model', type=str, help='Path to the pretrained model')
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_arguments()
    main(pretrained_model_path=args.model)