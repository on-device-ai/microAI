import tflite
import microai
import array
import sys

tflite.initialize_target();
tflite.setup_error_reporter();
if microai.load_model_from_file('/userdata/trained.tflite') is False :
  sys.exit()
tflite.build_interpreter()
if tflite.allocate_tensors() is False :
  sys.exit()
  
img_w    = 96
img_h    = 96
img_ch   = 3
img_size = img_w * img_h * img_ch

input_tensor  = array.array('b',[0] * img_size)
output_tensor = array.array('b',[0,0,0])

while True :
  microai.get_image(img_w,img_h,img_ch,input_tensor)
  microai.set_input_tensor_value(input_tensor)
  if tflite.interpreter_invoke() is False :
    sys.exit()
  microai.get_output_tensor_value(output_tensor) 
  lamp_score = output_tensor[0]
  plant_score = output_tensor[1]
  unknown_score = output_tensor[2]
  print("lamp_score = %d , plant_score = %d , unknown_score = %d" % (lamp_score,plant_score,unknown_score))