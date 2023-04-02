import numpy as np
import cv2
from tensorflow import keras
import tensorflow as tf
import sys
import time
import SPOTPoseNet
import os
import CustomVideoDataGenerator
import CustomImageDataGenerator
import matplotlib.pyplot as plt
import math
from datetime import datetime
import socket
import recordStereo 
import struct

outputYawSize = 128
image_size = 448
stop_count = 500
x_median = 1.46221606
y_median = -0.05725426


##################################################
        #### Load SPOTNet Weights  #####
##################################################

print(tf.__version__)
keras.backend.clear_session()
path_to_weights = sys.argv[1]

print("loading with leaky relu")
model = keras.models.load_model(path_to_weights,  custom_objects={'LeakyReLU': tf.keras.layers.LeakyReLU,
                                                                     'custom_loss': SPOTPoseNet.custom_loss,
                                                                     'custom_metric': SPOTPoseNet.custom_metric})
model.summary()
zedCamera = recordStereo.StereoCamera(1, 'MJPG', 30.0, 2560, 720)
zedCamera.initImageRecord()
left = np.zeros(shape=(1, image_size, image_size, 1))
right = np.zeros(shape=(1, image_size, image_size, 1))

while True:


    ##################################################
                #### Run SPOTNet  #####
    ##################################################
    
    start = time.time()

    image, timestamp = zedCamera.getImageAndTimeStamp()

    if len(image.shape) == 2:
        continue

    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    stereoFrame = np.split(image, 2, axis=1)

    left[:, :, :, 0] = cv2.resize(stereoFrame[0], (image_size, image_size))
    right[:, :, :, 0] = cv2.resize(stereoFrame[1], (image_size, image_size))

    stacked_stereo = np.concatenate((left, right), axis=3)

    output = model.predict(stacked_stereo)
    end = time.time()

    maxIdxVal = 0
    maxIdx = 0
    outputIndexs = output[2]

    for yawIdx in range(0, outputYawSize):
        if float(outputIndexs[0][yawIdx]) > maxIdxVal:
            maxIdxVal = float(outputIndexs[0][yawIdx])
            maxIdx = yawIdx

    yaw = ((maxIdx + 1) / outputYawSize) * 2 * math.pi - math.pi
    x = float(output[0] + x_median)
    y = float(output[1] + y_median)

    print("Network Output")
    print("X: " + str(abs(x)) + " Y: " + str(y) + " Yaw: " + str(yaw) + " Confidence: " + str(float(output[3])))
    print("\nInference Speed: ", str(end-start))
    
    try:
        # Create a UDP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Define the address and port to send to
        server_address = ('192.168.0.100', 30172)

        # Encode the data as a byte string
        data = bytearray(struct.pack("ffff", x, y, yaw, float(output[3])))

        # Send the data
        sock.sendto(data, server_address)
    except:
        print("Failed to Send Packet")

    #try:  # Try to receive data
    #    out_data = "SPOTNet\n" + str(x) + "\n" + str(y) + "\n" + str(yaw) + "\n" + str(float(output[3])) + "\n"
    #    client_socket.send(out_data.encode())
    #except:  # If receive fails, we have lost communication with the JetsonRepeater
    #    print("Lost communication with JetsonRepeater")
    #    connected = False
    #    continue  # Restart from while True

print("\n\nCompleted\n\n")
