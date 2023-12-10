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

class_names = ["1", "2", "9"]

train_ds = tf.keras.utils.image_dataset_from_directory(
    directory = "digits",
    labels="inferred",
    class_names=class_names,
    seed=None,
    image_size=(28, 28)
)

aug_train_ds = train_ds.map(
    lambda x, y: (data_augmentation(x, training=True), y),
    num_parallel_calls=tf.data.AUTOTUNE
)

aug_train_ds = aug_train_ds.prefetch(tf.data.AUTOTUNE)

plt.figure(figsize=(10, 10))

print("Augmented dataset size: ", len(list(aug_train_ds)))
for images, labels in aug_train_ds.take(1):
    for i in range(3):
        print(len(images))
        ax = plt.subplot(3, 3, i + 1)
        plt.imshow(images[i].numpy().astype("uint8"))
        plt.title(class_names[labels[i]])
        plt.axis("off")
        # plt.show()