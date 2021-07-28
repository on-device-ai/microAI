/******************************************************
	project	: microAI Library
	module	: The image provider functions
	author	: Yi-Lin Tung
	version	:
	date 	:
	Copyright (c) On-Device AI Co., Ltd.
******************************************************/

#include "libmicroai.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

#include <opencv2/opencv.hpp>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
  void   *start;
  size_t length;
};

static struct v4l2_buffer  m_buf;
static int                 m_fd = -1;
static struct buffer       *m_buffers;
static unsigned int        m_n_buffers;
static uint8_t             m_cap_init = 0;

static microai_result xioctl(int fh, int request, void *arg)
{
  int r;

  do {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

  if (r == -1) {
    return RSLT_FAIL;
  }

  return RSLT_FAIL;
}

#define VIDDEV_SCALE1   "/dev/video15"
#define CAPTURE_WIDTH   640
#define CAPTURE_HEIGHT  360
#define CAPTURE_FORMAT  V4L2_PIX_FMT_NV12

static microai_result capture_start() {

  char                       *dev_name = VIDDEV_SCALE1;
  struct v4l2_requestbuffers req;
  static struct v4l2_format  fmt;
  enum v4l2_buf_type         type;  
    
  m_fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
  if (m_fd < 0) {
     return RSLT_FAIL;
  }

  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = CAPTURE_WIDTH;
  fmt.fmt.pix.height      = CAPTURE_HEIGHT;
  fmt.fmt.pix.pixelformat = CAPTURE_FORMAT;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  xioctl(m_fd, VIDIOC_S_FMT, &fmt);
  if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_NV12) {
    return RSLT_FAIL;
  }

  CLEAR(req);
  req.count = 2;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  xioctl(m_fd, VIDIOC_REQBUFS, &req);

  m_buffers = (struct buffer *)calloc(req.count, sizeof(*m_buffers));
  for (m_n_buffers = 0; m_n_buffers < req.count; ++m_n_buffers) {
    CLEAR(m_buf);

    m_buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_buf.memory      = V4L2_MEMORY_MMAP;
    m_buf.index       = m_n_buffers;

    xioctl(m_fd, VIDIOC_QUERYBUF, &m_buf);

    m_buffers[m_n_buffers].length = m_buf.length;
    m_buffers[m_n_buffers].start = v4l2_mmap(NULL, m_buf.length,
                                         PROT_READ | PROT_WRITE, MAP_SHARED,
                                         m_fd, m_buf.m.offset);

    if (MAP_FAILED == m_buffers[m_n_buffers].start) {
      return RSLT_FAIL;
    }
  }

  for (int i = 0; i < m_n_buffers; ++i) {
    CLEAR(m_buf);
    m_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_buf.memory = V4L2_MEMORY_MMAP;
    m_buf.index = i;
    xioctl(m_fd, VIDIOC_QBUF, &m_buf);
  }
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  xioctl(m_fd, VIDIOC_STREAMON, &type);
  
  return RSLT_SUCCESS;
  
}

#define GRAY_CHANNELS 1
#define RGB_CHANNELS  3

microai_result microai_person_detection_example_GetImage(int image_width,int image_height, int channels, int8_t* image_data) {

  int             r;
  fd_set          fds;
  struct timeval  tv;
  FILE            *fout; 
  char            out_name[256];

  int             nWidth = CAPTURE_WIDTH;
  int             nHeight = CAPTURE_HEIGHT;

  cv::Mat         picNV12;
  cv::Mat         picBGR;

  uint8_t         *byte_array; 
  uint32_t        byte_array_len;

  if(m_cap_init == 0) {
    
    if(capture_start() == RSLT_SUCCESS) {
      m_cap_init = 1;
    } else {
      return RSLT_FAIL;
    }
  
  }

  do {

    FD_ZERO(&fds);
    FD_SET(m_fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(m_fd + 1, &fds, NULL, NULL, &tv);

  } while ((r == -1 && (errno = EINTR)));

  if (r == -1) {
    return RSLT_FAIL;
  }

  CLEAR(m_buf);
  m_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  m_buf.memory = V4L2_MEMORY_MMAP;
  xioctl(m_fd, VIDIOC_DQBUF, &m_buf);

  picNV12 = cv::Mat(nHeight * 3/2, nWidth, CV_8UC1, m_buffers[m_buf.index].start);

  cv::cvtColor(picNV12, picBGR, CV_YUV2BGR_NV12);

  if( channels == GRAY_CHANNELS ) {

    cv::Mat picGray;
    cv::Mat picGray_resize;

    cv::cvtColor(picBGR, picGray, CV_BGR2GRAY);

    cv::resize( picGray, picGray_resize, cv::Size(image_width, image_height));

    byte_array = picGray_resize.isContinuous() ? picGray_resize.data : picGray_resize.clone().data;

    byte_array_len = picGray_resize.total() * picGray_resize.channels();

  } else if( channels == RGB_CHANNELS ) {

    cv::Mat picRGB;
    cv::Mat picRGB_resize;

    cv::cvtColor(picBGR, picRGB, CV_BGR2RGB);

    cv::resize( picRGB, picRGB_resize, cv::Size(image_width, image_height));

    byte_array = picRGB_resize.isContinuous() ? picRGB_resize.data : picRGB_resize.clone().data;

    byte_array_len = picRGB_resize.total() * picRGB_resize.channels();

    
  } else {

    xioctl(m_fd, VIDIOC_QBUF, &m_buf);
    
    return RSLT_FAIL;

  }

  for(int i=0;i<byte_array_len;i++) {
    image_data[i] = (int8_t)(byte_array[i] - 128);
  }

  xioctl(m_fd, VIDIOC_QBUF, &m_buf);

  return RSLT_SUCCESS;
}

void microai_capture_stop() {
   
   if( m_cap_init == 1 ) {
 
     enum v4l2_buf_type type;  

     type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
     xioctl(m_fd, VIDIOC_STREAMOFF, &type);
     for (int i = 0; i < m_n_buffers; ++i) {
       v4l2_munmap(m_buffers[i].start, m_buffers[i].length);
     }
     v4l2_close(m_fd);

     m_cap_init = 0;
   
   }
   
}

void image_data_to_bmp(int image_width,int image_height, int channels, int8_t* image_data,char* bmp_file_name) {

  int       image_data_size = image_width * image_height * channels;
  uint8_t * byte_array = (uint8_t *) malloc( image_data_size );
  cv::Mat   picImage;

  if( byte_array ) {

    for(int i=0;i<image_data_size;i++) {
      byte_array[i] = (uint8_t)(image_data[i] + 128);
    }

    if( channels == GRAY_CHANNELS ) {

      picImage = cv::Mat(image_height , image_width, CV_8UC1, byte_array);

    } else if( channels == RGB_CHANNELS ) {

      cv::Mat picRGB = cv::Mat(image_height , image_width, CV_8UC3, byte_array);

      // Bear in mind that OpenCV represents color values in the BGR order.
      cv::cvtColor(picRGB, picImage, CV_RGB2BGR);

    }

    cv::imwrite(bmp_file_name, picImage);    

    free(byte_array);
  }

}
