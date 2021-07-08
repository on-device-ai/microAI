/******************************************************
	project	: microAI Library
	module	: person_detection unit test progarm
	author	: Yi-Lin Tung
	version	:
	date 	:
	Copyright (c) On-Device AI Co., Ltd.
******************************************************/

#include <stdio.h>
#include "libmicroai.h"
#include "person_detection_model.h"

#define IMAGE_WIDTH     96
#define IMAGE_HEIGHT    96
#define IMAGE_CHANNEL   1

void setup();
void loop();

int main(int argc, char* argv[]) {
  setup();
  while (1) {
    loop();
  }
}

void setup() {
  tflite_InitializeTarget();
  tflite_setup_MicroErrorReporter();
  if(tflite_GetModel(person_detection_model) == RSLT_FAIL) {
    return;
  }
  tflite_build_MicroInterpreter();
  if(tfilte_AllocateTensors() == RSLT_FAIL) {
    return;
  }
}

void loop() {

  microai_tensor input;
  microai_tensor output;

  int8_t person_score;
  int8_t no_person_score;

  microai_get_input_tensor(&input);
  microai_get_output_tensor(&output);

  if(microai_person_detection_example_GetImage(IMAGE_WIDTH,IMAGE_HEIGHT,IMAGE_CHANNEL, input.data.int8) != RSLT_SUCCESS) {
    printf("Image capture failed.\n");
    return;
  } 

  if(tflite_interpreterInvoke() == RSLT_FAIL) {
    printf("Invoke failed.\n");
    return;
  }

  person_score = output.data.int8[1];
  no_person_score = output.data.int8[0];

  printf("person score:%d no person score %d\n",person_score, no_person_score);
}



