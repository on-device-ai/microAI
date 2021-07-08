/******************************************************
	project	: microAI Library
	module	: The tflite-micro library's wrapper functions
	author	: Yi-Lin Tung
	version	:
	date 	:
	Copyright (c) On-Device AI Co., Ltd.
******************************************************/

#include "libmicroai.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 64*1024*1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

void tflite_InitializeTarget() {
  tflite::InitializeTarget();
}

void tflite_setup_MicroErrorReporter() {
	
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;
  
}

microai_result tflite_GetModel(const void *buf) {
	
  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(buf);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return RSLT_FAIL;
  }	

  return RSLT_SUCCESS;
  
}

void tflite_build_MicroInterpreter() {
	
  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;
	
}

microai_result tfilte_AllocateTensors() {

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return RSLT_FAIL;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
  
  return RSLT_SUCCESS;
  
}	
	
microai_result tflite_interpreterInvoke() {

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
    return RSLT_FAIL;
  }

  return RSLT_SUCCESS;
	
}

void microai_get_input_tensor(microai_tensor *input_tensor) {

	input_tensor->data.data        = input->data.data;
	input_tensor->param_scale      = input->params.scale;
	input_tensor->param_zero_point = input->params.zero_point;
	
}

void microai_get_output_tensor(microai_tensor *output_tensor) {
	
	output_tensor->data.data        = output->data.data;
	output_tensor->param_scale      = output->params.scale;
	output_tensor->param_zero_point = output->params.zero_point;
	
}

void microai_hello_world_example_HandleOutput(float x_value,float y_value) {
  TF_LITE_REPORT_ERROR(error_reporter, "x_value: %f, y_value: %f\n",
                       static_cast<double>(x_value),
                       static_cast<double>(y_value));
}
