/******************************************************
	project	: microAI Library
	module	: hello_world unit test progarm
	author	: Yi-Lin Tung
	version	:
	date 	:
	Copyright (c) On-Device AI Co., Ltd.
******************************************************/

#include <stdio.h>
#include "libmicroai.h"
#include "hello_world_model.h"

void setup();
void loop();

int inference_count = 0;

const int kInferencesPerCycle = 20;
const float kXrange = 2.f * 3.14159265359f;

int main(int argc, char* argv[]) {
  setup();
  while (1) {
    loop();
  }
}

void setup() {
  tflite_InitializeTarget();
  tflite_setup_MicroErrorReporter();
  if(tflite_GetModel(hello_world_model) == RSLT_FAIL) {
    return;
  }
  tflite_build_MicroInterpreter();
  if(tfilte_AllocateTensors() == RSLT_FAIL) {
    return;
  }
  inference_count = 0;
}

void loop() {

  microai_tensor input;
  microai_tensor output;
  
  float   position;
  float   x;
  int8_t  x_quantized;
  
  int8_t  y_quantized;
  float   y;
  
  microai_get_input_tensor(&input);
  microai_get_output_tensor(&output);
  
  position = ((float)inference_count) / ((float)kInferencesPerCycle);
  x = position * kXrange;
  
  x_quantized = x / input.param_scale + input.param_zero_point;
  input.data.int8[0] = x_quantized;
  
  if(tflite_interpreterInvoke() == RSLT_FAIL) {
    printf("Invoke failed on x: %f\n",x);
    return;
  }
  
  y_quantized = output.data.int8[0];
  y = (y_quantized - output.param_zero_point) * output.param_scale;

  microai_hello_world_example_HandleOutput(x,y);
  printf("<= x: %f, y: %f\n\n",x,y);
  
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) {
    inference_count = 0;
  }
}



