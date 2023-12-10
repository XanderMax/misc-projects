import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt
import time

data_augmentation = keras.Sequential(
    [
        layers.RandomTranslation(height_factor=0.1, width_factor=0.1),
        layers.RandomRotation(0.1),
    ]
)

class_names = ["1", "2", "3", "9"]

train_ds = tf.keras.utils.image_dataset_from_directory(
    directory = "dataset",
    labels="inferred",
    class_names=class_names,
    batch_size=10000,
    seed=None,
    image_size=(28, 28)
)

aug_train_ds = train_ds.map(
    lambda x, y: (data_augmentation(x, training=True), y),
    num_parallel_calls=tf.data.AUTOTUNE
)

aug_train_ds = aug_train_ds.prefetch(tf.data.AUTOTUNE)


dataset_len = len(list(aug_train_ds))
print("Augmented dataset size: ", dataset_len)
print(aug_train_ds)

model = tf.keras.Sequential([
    tf.keras.layers.Flatten(input_shape=(28, 28, 3)),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(4)
])

model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])

for images, labels in aug_train_ds.take(dataset_len):
    model.fit(images, labels, epochs=10)

probability_model = tf.keras.Sequential([model, 
                                         tf.keras.layers.Softmax()])


test_ds = tf.keras.utils.image_dataset_from_directory(
    directory = "digits-test",
    labels="inferred",
    class_names=["query"],
    batch_size=1,
    seed=None,
    image_size=(28, 28)
)

for i in range(10):
    for image, label in test_ds.take(1):
        predictions = probability_model.predict(image)
        print(predictions)
        print(class_names[tf.argmax(predictions[0])])