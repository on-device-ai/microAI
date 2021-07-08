import tflite
import microai
import sys

tflite.initialize_target();
tflite.setup_error_reporter();
if microai.load_person_detection_model() is False :
  sys.exit()
tflite.build_interpreter()
if tflite.allocate_tensors() is False :
  sys.exit()
  
while True :
  input_tensor = microai.get_image()
  microai.set_input_tensor_int8(input_tensor)
  if tflite.interpreter_invoke() is False :
    sys.exit()
  output_tensor = microai.get_output_tensor_int8(2) 
  person_score = microai.value_byte_to_int(output_tensor[1])
  no_person_score = microai.value_byte_to_int(output_tensor[0])
  if person_score > no_person_score :
    print("Person detected!")
  else:
    print("No person detected.")
  