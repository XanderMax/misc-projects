import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '4'  # or any {'0', '1', '2'}
import argparse
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

def main(pretrained_model_path, image_file_path, output_file_path):
    print(pretrained_model_path)
    print(image_file_path)
    model = tf.keras.models.load_model(pretrained_model_path)
    image = load_image(image_file_path)
    predictions = model.predict(image)
    prediction = np.argmax(predictions[0])
    print(prediction)
    print(predictions)
    with open(output_file_path, 'w') as f:
        f.write(str(prediction))

def load_image(filename):
    image = tf.keras.utils.load_img(filename,target_size=(28,28), color_mode='grayscale')
    input_arr = tf.keras.utils.img_to_array(image)

    input_arr = 1 - np.array(input_arr).astype('float32')/255
    input_arr = np.array([input_arr])
    return input_arr

def parse_arguments():
    parser = argparse.ArgumentParser(description='Image Classifier')
    parser.add_argument('--model', type=str, help='Path to the pretrained model')
    parser.add_argument('--image', type=str, help='Path to the image file')
    parser.add_argument('--output', type=str, help='Path to the image file')
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_arguments()
    main(pretrained_model_path=args.model, image_file_path=args.image, output_file_path=args.output)