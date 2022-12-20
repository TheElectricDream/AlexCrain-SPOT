import os
os.environ["PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION"] = "python"

import tensorflow as tf
import csv

# Load the saved model or graph
export_dir = os.path.join('export_dir', '0')

model = tf.saved_model.load(export_dir,tags=["serve"],export_dir=export_dir)

objects = tf.saved_model.list_objects(export_dir)
print(objects)
model = tf.compat.v1.saved_model.load(export_dir, tags=["serve"], export_dir=export_dir)

# Find the tensors for the weights and biases
weights = model.graph.get_tensor_by_name("weights:0")
biases = model.graph.get_tensor_by_name("biases:0")

# Create a session to evaluate the tensors
with model.graph.as_default():
    with tf.compat.v1.Session() as sess:
        # Get the values of the weights and biases
        weights_val, biases_val = sess.run([weights, biases])

# Write the values to a CSV file
with open("weights.csv", "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerows(weights_val)

with open("biases.csv", "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(biases_val)