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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libmicroai.h"

#include "hello_world_model.h"
#include "person_detection_model.h"

#include "py/obj.h"
#include "py/runtime.h"

// Use special typecode to differentiate repr() of bytearray vs array.array('B')
// (underlyingly they're same).  Can't use 0 here because that's used to detect
// type-specification errors due to end-of-string.
#define BYTEARRAY_TYPECODE 1

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

#define MAX_MODEL_SIZE (64*1024*1024)
static uint8_t model_buffer[MAX_MODEL_SIZE];

STATIC mp_obj_t microai_load_model_from_file(mp_obj_t filename) {

  microai_result _result;

  const char *_filename = mp_obj_str_get_str(filename);

  int _fd;

  struct stat _sb;

  _fd = open( _filename, O_RDONLY);
  if( _fd == -1 ) {

    return mp_const_false;

  }

  if( stat( _filename, &_sb) == -1 ) {

    return mp_const_false;

  }

  if( _sb.st_size > MAX_MODEL_SIZE ) {

    return mp_const_false;

  }

  memset( model_buffer, 0x00, sizeof(model_buffer));

  read( _fd, model_buffer, _sb.st_size);

  close(_fd);

  _result = tflite_GetModel( (const unsigned char *)model_buffer );

  return _result == RSLT_SUCCESS ? mp_const_true : mp_const_false ;
  
}

STATIC mp_obj_t microai_set_input_tensor_value(mp_obj_t tensor) {

  microai_tensor   _input;
  mp_buffer_info_t _bufinfo;
  
  microai_get_input_tensor(&_input);  

  if (mp_get_buffer(tensor, &_bufinfo, MP_BUFFER_READ)) {

    switch(_bufinfo.typecode) {
      
      case BYTEARRAY_TYPECODE:
      case 'B':
          memcpy(_input.data.uint8,(uint8_t *)_bufinfo.buf,_bufinfo.len);
          break;

      case 'b':
          memcpy(_input.data.int8,(int8_t *)_bufinfo.buf,_bufinfo.len);
          break;

      case 'f':
          memcpy(_input.data.f,(float *)_bufinfo.buf,_bufinfo.len);
          break;

      default:
          return mp_const_false;

    }

    return mp_const_true;

  }
    
  return mp_const_false;

} 

STATIC mp_obj_t microai_get_output_tensor_value(mp_obj_t tensor) {

  microai_tensor   _output;
  mp_buffer_info_t _bufinfo;

  microai_get_output_tensor(&_output);

  if (mp_get_buffer(tensor, &_bufinfo, MP_BUFFER_WRITE)) {

    switch(_bufinfo.typecode) {
      
      case BYTEARRAY_TYPECODE:
      case 'B':
          memcpy((uint8_t *)_bufinfo.buf,_output.data.uint8,_bufinfo.len);
          break;

      case 'b':
          memcpy((int8_t *)_bufinfo.buf,_output.data.int8,_bufinfo.len);
          break;

      case 'f':
          memcpy((float *)_bufinfo.buf,_output.data.f,_bufinfo.len);
          break;

      default:
          return mp_const_false;

    }

    return mp_const_true;

  }    


  return mp_const_false;
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

STATIC mp_obj_t microai_get_image(mp_uint_t n_args, const mp_obj_t *args) {

  int              _image_width;
  int              _image_height;
  int              _image_channels;
  mp_buffer_info_t _bufinfo;

  (void)n_args;

  _image_width    = mp_obj_get_int(args[0]);
  _image_height   = mp_obj_get_int(args[1]);
  _image_channels = mp_obj_get_int(args[2]);

  if (mp_get_buffer(args[3], &_bufinfo, MP_BUFFER_WRITE)) {

    if( (_bufinfo.typecode == 'b') && ((_image_width * _image_height * _image_channels) == _bufinfo.len) ) {

      if(microai_person_detection_example_GetImage(_image_width, _image_height, _image_channels,(int8_t*)_bufinfo.buf) == RSLT_SUCCESS) {
         return mp_const_true;
      }

    }

  }

  return mp_const_false;
} 

STATIC const MP_DEFINE_CONST_FUN_OBJ_0(microai_obj_load_hello_world_model,microai_load_hello_world_model);
STATIC const MP_DEFINE_CONST_FUN_OBJ_0(microai_obj_load_person_detection_model,microai_load_person_detection_model);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_load_model_from_file,microai_load_model_from_file);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_set_input_tensor_value,microai_set_input_tensor_value);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_get_output_tensor_value,microai_get_output_tensor_value);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_value_input_quantize,microai_value_input_quantize);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(microai_obj_value_output_dequantize,microai_value_output_dequantize);
STATIC const MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(microai_obj_get_image,4,4,microai_get_image);

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
  {MP_OBJ_NEW_QSTR(MP_QSTR_load_model_from_file), MP_ROM_PTR(&microai_obj_load_model_from_file)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_set_input_tensor_value), MP_ROM_PTR(&microai_obj_set_input_tensor_value)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_get_output_tensor_value), MP_ROM_PTR(&microai_obj_get_output_tensor_value)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_value_input_quantize), MP_ROM_PTR(&microai_obj_value_input_quantize)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_value_output_dequantize), MP_ROM_PTR(&microai_obj_value_output_dequantize)},
  {MP_OBJ_NEW_QSTR(MP_QSTR_get_image), MP_ROM_PTR(&microai_obj_get_image)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_microai_globals, microai_globals_table);   

const mp_obj_module_t mp_module_microai = {
  .base = {&mp_type_module},    
  .globals = (mp_obj_dict_t *)&mp_module_microai_globals,
};

