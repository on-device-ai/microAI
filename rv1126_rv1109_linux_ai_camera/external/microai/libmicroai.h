/*-----------------------------------------------------
  project	 : microAI Library
  filename : libmicroai.h
  author	 : Yi-Lin Tung
  version	 : 
  date 	 : 
  Copyright (c) On-Device AI Co., Ltd.
-----------------------------------------------------*/

#ifndef LIBMICROAI_H
#define LIBMICROAI_H 

#include <stdint.h>

typedef enum{RSLT_SUCCESS, RSLT_FAIL} microai_result;

typedef union MicroaiPtrUnion {
  float*   f;
  uint8_t* uint8;
  int8_t*  int8;
  void*    data;
} microai_ptr_union;

typedef struct MicroaiTensor {
  microai_ptr_union data;
  float             param_scale;
  int32_t           param_zero_point;
} microai_tensor;

#ifdef __cplusplus
extern "C" {
#endif

void tflite_InitializeTarget();
void tflite_setup_MicroErrorReporter();
microai_result tflite_GetModel(const void *buf);
void tflite_build_MicroInterpreter();
microai_result tfilte_AllocateTensors();
microai_result tflite_interpreterInvoke();

void microai_get_input_tensor(microai_tensor *input_tensor);
void microai_get_output_tensor(microai_tensor *output_tensor);

void microai_hello_world_example_HandleOutput(float x_value,float y_value);

microai_result microai_person_detection_example_GetImage(int image_width,int image_height, int channels, int8_t* image_data);
void microai_capture_stop();
void image_data_to_bmp(int image_width,int image_height, int channels, int8_t* image_data,char* bmp_file_name);


#ifdef __cplusplus
}
#endif

#endif
