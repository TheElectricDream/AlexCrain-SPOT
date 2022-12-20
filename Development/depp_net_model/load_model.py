import os
import tensorflow as tf
from tensorflow.python.platform import gfile

GRAPH_PB_PATH = './export_dir/0/saved_model.pb' #path to your .pb file
graph = tf.Graph()
with tf.compat.v1.Session(graph=graph) as sess:
  print("load graph")
  with tf.compat.v1.gfile.GFile(GRAPH_PB_PATH,'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())
    sess.graph.as_default()
    tf.compat.v1.import_graph_def(graph_def, name='')
    graph_nodes=[n for n in graph_def.node]