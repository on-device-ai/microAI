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

#define IMAGE_WIDTH     96
#define IMAGE_HEIGHT    96
#define IMAGE_CHANNEL   1

#define IMAGE_BUFF_LEN  IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_CHANNEL

static int8_t ImageBuff[IMAGE_BUFF_LEN];

int main(int argc, char* argv[]) {

  int  i = 0;
  char bmp_name[256];

  for(i=0;i<10;i++) {
    
    if( microai_person_detection_example_GetImage(IMAGE_WIDTH,IMAGE_HEIGHT, IMAGE_CHANNEL, ImageBuff) == RSLT_FAIL ) {
       break;
    }

    sprintf(bmp_name, "/userdata/test%03d.bmp", i);
    image_data_to_bmp(IMAGE_WIDTH,IMAGE_HEIGHT, IMAGE_CHANNEL, ImageBuff, bmp_name);
    
  }

  microai_capture_stop();
}

  
