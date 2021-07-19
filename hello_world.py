import tflite
import microai
import array
import sys

inferences_per_cycle = 20;
x_range = 2.0 * 3.14159265359;

tflite.initialize_target();
tflite.setup_error_reporter();
if microai.load_hello_world_model() is False :
  sys.exit()
tflite.build_interpreter()
if tflite.allocate_tensors() is False :
  sys.exit()
  
inference_count = 0;

while True :
  position = float(inference_count) / float(inferences_per_cycle)
  x = position * x_range
  input_tensor = array.array('b',[microai.value_input_quantize(x)])
  microai.set_input_tensor_value(input_tensor)
  if tflite.interpreter_invoke() is False :
    sys.exit()
  output_tensor = array.array('b',[0])
  microai.get_output_tensor_value(output_tensor)
  y = microai.value_output_dequantize(output_tensor[0])
  inference_count += 1
   
  result_str = " x = %f , y = %f " % (x,y)
  print(result_str)
   
  if inference_count >= inferences_per_cycle :
    inference_count = 0;