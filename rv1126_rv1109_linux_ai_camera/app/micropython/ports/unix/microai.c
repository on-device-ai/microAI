/******************************************************
	project	: MicroPython microAI module
	module	: mp_module_microai
	author	: Yi-Lin Tung
	version	:
	date 	:
	Copyright (c) On-Device AI Co., Ltd.
******************************************************/

#include <stdio.h>
#include <string.h>

#include "libmicroai.h"

#include "hello_world_model.h"
#include "person_detection_model.h"

#include "py/obj.h"
#include "py/runtime.h"

/***** Function *****/

/* tflite */

STATIC mp_obj_t tflite_initialize_target() {

  tflite_InitializeTarget();

  return mp_const_none;
}

STATIC mp_obj_t tflite_setup_error_reporter() {

  tflite_setup_MicroErrorReporter();

  return mp_const_none;
}

STATIC mp_obj_t tflite_build_interpreter() {

  tflite_build_MicroInterpreter();

  return mp_const_none;
}

STATIC mp_obj_t tflite_allocate_tensors() {
  
  microai_result _result;

  _result = tfilte_AllocateTensors();

  return _result == RSLT_SUCCESS ? mp_const_true : mp_const_false ;
  
}

STATIC mp_obj_t tflite_interpreter_invoke() {

  microai_result _result;

  _result = tflite_interpreterInvoke();

 return _result == RSLT_SUCCESS ? mp_const_true : mp_const_false ;
  
}

STATIC const MP_DEFINE_CONST_FUN_OBJ_0(tflite_obj_initialize_target,tflite_initialize_target);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(tflite_obj_setup_error_reporter,tflite_setup_error_reporter);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(tflite_obj_build_interpreter,tflite_build_interpreter);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(tflite_obj_allocate_tensors,tflite_allocate_tensors);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(tflite_obj_interpreter_invoke,tflite_interpreter_invoke);

/* microai */

STATIC mp_obj_t microai_load_hello_world_model() {

  microai_result _result;

  _result = tflite_GetModel(hello_world_model);

  return _result == RSLT_SUCCESS ? mp_const_true : mp_const_false ;
  
}


STATIC mp_obj_t microai_load_person_detection_model() {

  microai_result _result;

  _result = tflite_GetModel(person_detection_model);

  return _result == RSLT_SUCCESS ? mp_const_true : mp_const_false ;
  
}

STATIC mp_obj_t microai_set_input_tensor_int8(mp_obj_t data) {

  microai_tensor   _input;
  mp_buffer_info_t _bufinfo;
  
  microai_get_input_tensor(&_input);  


  if (mp_get_buffer(data, &_bufinfo, MP_BUFFER_WRITE)) {

    memcpy(_input.data.int8,(int8_t *)_bufinfo.buf,_bufinfo.len);

    return mp_const_true;

  }
    
  return mp_const_false;
} 



STATIC mp_obj_t microai_get_output_tensor_int8(mp_obj_t len) {

  microai_tensor _output;

  size_t         _len;

  _len = mp_obj_get_int(len);

  microai_get_output_tensor(&_output);

  return mp_obj_new_bytearray(_len, _output.data.int8);
  
}

STATIC mp_obj_t microai_value_input_quantize(mp_obj_t value) {

  microai_tensor _input;
  float          _value;
  int8_t         _value_quantized;

  microai_get_input_tensor(&_input);

  _value = mp_obj_get_float(value);

  _value_quantized = _value / _input.param_scale + _input.param_zero_point;

  return mp_obj_new_int(_value_quantized);
  
}

STATIC mp_obj_t microai_value_output_dequantize(mp_obj_t value_quantized) {

  microai_tensor _output;
  int8_t         _value_quantized;
  float          _value;
  
  microai_get_output_tensor(&_output);

  _value_quantized = mp_obj_get_int(value_quantized);

  _value =  (_value_quantized - _output.param_zero_point) * _output.param_scale;

  return mp_obj_new_float(_value);
  
}

STATIC mp_obj_t microai_value_byte_to_int(mp_obj_t byte_value) {

  int8_t _byte_value;
  
  _byte_value = mp_obj_get_int(byte_value);
  
  return mp_obj_new_int((int32_t)_byte_value);
}


#define IMAGE_WIDTH     96
#define IMAGE_HEIGHT    96
#define IMAGE_CHANNEL   1

static int8_t m_image_buffer[IMAGE_WIDTH*IMAGE_HEIGHT*IMAGE_CHANNEL];

STATIC mp_obj_t microai_get_image() {

  if(microai_person_detection_example_GetImage(IMAGE_WIDTH,IMAGE_HEIGHT,IMAGE_CHANNEL, m_image_buffer) != RSLT_SUCCESS) {
    return mp_const_none;
  }

  return mp_obj_new_bytearray(sizeof(m_image_buffer)/sizeof(int8_t), m_image_buffer);

} 

STATIC const MP_DEFINE_CONST_FUN_OBJ_0(microai_obj_load_hello_world_model,microai_load_hello_world_model);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(microai_obj_load_person_detection_model,microai_load_person_detection_model);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_set_input_tensor_int8,microai_set_input_tensor_int8);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_get_output_tensor_int8,microai_get_output_tensor_int8);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_value_input_quantize,microai_value_input_quantize);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_value_output_dequantize,microai_value_output_dequantize);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(microai_obj_get_image,microai_get_image);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_value_byte_to_int,microai_value_byte_to_int);

/***** Module *****/

/* tflite */

STATIC const mp_rom_map_elem_t tflite_globals_table[] = {
  {MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_tflite)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_initialize_target), MP_ROM_PTR(&tflite_obj_initialize_target)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_setup_error_reporter), MP_ROM_PTR(&tflite_obj_setup_error_reporter)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_build_interpreter), MP_ROM_PTR(&tflite_obj_build_interpreter)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_allocate_tensors), MP_ROM_PTR(&tflite_obj_allocate_tensors)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_interpreter_invoke), MP_ROM_PTR(&tflite_obj_interpreter_invoke)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_tflite_globals, tflite_globals_table);   

const mp_obj_module_t mp_module_tflite = {
  .base = {&mp_type_module},    
  .globals = (mp_obj_dict_t *)&mp_module_tflite_globals,
};

/* microai */

STATIC const mp_rom_map_elem_t microai_globals_table[] = {
  {MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_microai)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_load_hello_world_model), MP_ROM_PTR(&microai_obj_load_hello_world_model)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_load_person_detection_model), MP_ROM_PTR(&microai_obj_load_person_detection_model)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_set_input_tensor_int8), MP_ROM_PTR(&microai_obj_set_input_tensor_int8)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_get_output_tensor_int8), MP_ROM_PTR(&microai_obj_get_output_tensor_int8)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_value_input_quantize), MP_ROM_PTR(&microai_obj_value_input_quantize)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_value_output_dequantize), MP_ROM_PTR(&microai_obj_value_output_dequantize)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_get_image), MP_ROM_PTR(&microai_obj_get_image)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_value_byte_to_int), MP_ROM_PTR(&microai_obj_value_byte_to_int)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_microai_globals, microai_globals_table);   

const mp_obj_module_t mp_module_microai = {
  .base = {&mp_type_module},    
  .globals = (mp_obj_dict_t *)&mp_module_microai_globals,
};

