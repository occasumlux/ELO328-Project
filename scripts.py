from __future__ import print_function

import numpy as np
import tensorflow as tf
from tensorflow import keras
from PIL import Image

def predict(imFilename):
    """
    Function that opens a saved model and uses it
    to get the probability of an image to be in the classes
    defined inside it.
    """
    json_file = open('model.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = keras.models.model_from_json(loaded_model_json)
    loaded_model.load_weights("model.h5")
    loaded_model.compile(optimizer=tf.train.AdamOptimizer(), 
                         loss='sparse_categorical_crossentropy',
                         metrics=['accuracy'])
    pic = np.array(Image.open(imFilename))
    test_image = np.array([pic])
    predictions = loaded_model.predict(test_image)
    return np.amax(predictions) # Return only the maximum

def train(imageSize):
    print("Training model")

    classes = ["Palm1", "Palm2", "Palm3"]

    num_train_each = 20

    pic = np.array(Image.open("train/palm1_f" + str(0) + ".jpg"))
    train_images = np.array([pic])
    train_labels = np.array([x for x in range(len(classes)) for i in range(num_train_each)])

    for i in range(1, num_train_each):
        pic = np.array(Image.open("train/palm1_f" + str(i) + ".jpg"))
        train_images = np.vstack((train_images, np.array([pic])))

    for i in range(num_train_each):
        pic = np.array(Image.open("train/palm2_f" + str(i) + ".jpg"))
        train_images = np.vstack((train_images, np.array([pic])))
    
    for i in range(num_train_each):
        pic = np.array(Image.open("train/palm3_f" + str(i) + ".jpg"))
        train_images = np.vstack((train_images, np.array([pic])))

    train_images = train_images / 255.0

    model = keras.Sequential([
        keras.layers.Flatten(input_shape=(imageSize, imageSize)),
        keras.layers.Dense(64, activation=tf.nn.relu),
        keras.layers.Dense(2, activation=tf.nn.softmax)
    ])

    model.compile(optimizer=tf.train.AdamOptimizer(), 
            loss='sparse_categorical_crossentropy',
            metrics=['accuracy'])

    model.fit(train_images, train_labels, epochs=5)
    
    # serialize model to JSON
    model_json = model.to_json()
    with open("model.json", "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    model.save_weights("model.h5")
    print("Saved model to disk")
    return 0
