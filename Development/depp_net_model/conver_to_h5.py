import tensorflow as tf
from tensorflow.keras.models import save_model, Sequential

model = tf.keras.models.load_model("./export_dir/0", compile=False)

tf.model.save(model,"./export_dir/0/new_model.h5", save_format='h5')