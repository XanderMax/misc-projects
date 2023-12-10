import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

mnist = tf.keras.datasets.mnist
(x_train,y_train) , (x_test,y_test) = mnist.load_data()

x_train = tf.keras.utils.normalize(x_train,axis=1)
x_test = tf.keras.utils.normalize(x_test,axis=1)

def draw(n):
	plt.imshow(n,cmap=plt.cm.binary)
	plt.show()

print(x_train[0].shape)
print(x_train[0])
draw(x_train[0])

def get_model():
    if os.path.isfile('epic_num_reader.h5'):
        return tf.keras.models.load_model('epic_num_reader.h5')

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

    model.save('epic_num_reader.h5')


def load_image(filename):
    image = tf.keras.utils.load_img(filename,target_size=(28,28), color_mode='grayscale')
    input_arr = tf.keras.utils.img_to_array(image)
    print(input_arr.shape)

    input_arr = 1 - np.array(input_arr).astype('float32')/255
    draw(input_arr)
    input_arr = np.array([input_arr])
    return input_arr

model = get_model()

print(sys.argv)



predictions=model.predict(load_image(sys.argv[1]))
print('prediction -> ',np.argmax(predictions[0]))