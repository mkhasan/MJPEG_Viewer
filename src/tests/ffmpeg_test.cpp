// tutorial01.c
// Code based on a tutorial by Martin Bohme (boehme@inb.uni-luebeckREMOVETHIS.de)
// Tested on Gentoo, CVS version 5/01/07 compiled with GCC 4.1.1
// With updates from https://github.com/chelyaev/ffmpeg-tutorial
// Updates tested on:
// LAVC 54.59.100, LAVF 54.29.104, LSWS 2.1.101 
// on GCC 4.7.2 in Debian February 2015

// A small sample program that shows how to use libavformat and libavcodec to
// read video from a file.
//
// Use
//
// gcc -o tutorial01 tutorial01.c -lavformat -lavcodec -lswscale -lz
//
// to build (assuming libavformat and libavcodec are correctly installed
// your system).
//
// Run using
//
// tutorial01 myvideofile.mpg
//
// to write the first five frames from "myvideofile.mpg" to disk in PPM
// format.

#include "ImageProcUtils.h"
#include "utils.h"
#include "Config.h"
#include "darknet.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#ifdef __cplusplus
}
#endif

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string>


// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif


using namespace std;
using namespace cv;
using namespace image_proc_utils;

static cv::Mat imageData;
static bool quit = false;
static std::unique_ptr<Detector> net;

void* detector_thread(void* arg);

class Data {
public:

	ImgData::image_header header;
	sem_t mutex_lock;
	Data();
	void SetData(const Mat& frame, int seq);
	void GetData(Mat& frame, ImgData::image_header& header);
};


static Data data;

Data::Data() {
	header.seq = -1;
	sem_init(&mutex_lock, 0, 1);
}

void Data::SetData(const Mat& frame, int seq) {

	sem_wait(&mutex_lock);

	imageData = frame.clone();
	header.seq = seq;
	header.imageSize = 1;

	//cout << "header.seq: " << header.seq << endl;
	sem_post(&mutex_lock);

}

void Data::GetData(Mat& frame, ImgData::image_header& header_) {
	sem_wait(&mutex_lock);
	frame = imageData.clone();
	header_ = header;
	sem_post(&mutex_lock);
}




void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;
  
  /*
  // Open file
  sprintf(szFilename, "temp/frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;
  
  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);
  
  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
  
  // Close file
  fclose(pFile);
  */
  cv::Mat frame(height, width, CV_8UC3, pFrame->data[0]);

  for (int y=0; y<frame.rows; y++) {
	  for (int x=0; x<frame.cols; x++) {
		  cv::Vec3b& color = frame.at<cv::Vec3b>(cv::Point(x,y));
		  auto temp = color[0];
		  color[0] = color[2];
		  color[2] = temp;
	  }
  }

  data.SetData(frame, iFrame-1);

  if (0) {

	  sprintf(szFilename, "temp/frame%06d.jpg", iFrame);
	  cv::imwrite(szFilename, frame);
  }
}

int main(int argc, char *argv[]) {

	try {
		ServiceContainer *serviceContainer = ServiceContainer::Instance();
		serviceContainer->set<DETECTION_SENDER>(SERVER_IP, 80);//DETECTION_READER_CUSTOM_PORT);
		net = std::make_unique<DarkNet>();

	}
	catch(const exception& exp) {
		cout << "Error: " << string(exp.what()) << endl;
		exit(1);
	}

	pthread_t t1;
	pthread_create(&t1,NULL,detector_thread ,NULL);
	// Initalizing these to NULL prevents segfaults!
	AVFormatContext   *pFormatCtx = NULL;
	int               i, videoStream;
	AVCodecContext    *pCodecCtxOrig = NULL;
	AVCodecContext    *pCodecCtx = NULL;
	AVCodec           *pCodec = NULL;
	AVFrame           *pFrame = NULL;
	AVFrame           *pFrameRGB = NULL;
	AVPacket          packet;
	int               frameFinished;
	int               numBytes;
	uint8_t           *buffer = NULL;
	struct SwsContext *sws_ctx = NULL;

	if(argc < 2) {
		printf("Please provide a movie file\n");
		return -1;
	}
	// Register all formats and codecs
	av_register_all();

	// Open video file
	if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
		return -1; // Couldn't open file

	// Retrieve stream information
	if(avformat_find_stream_info(pFormatCtx, NULL)<0)
		return -1; // Couldn't find stream information

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, argv[1], 0);

	// Find the first video stream
	videoStream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++)
	if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
		videoStream=i;
		break;
	}
	if(videoStream==-1)
		return -1; // Didn't find a video stream

	// Get a pointer to the codec context for the video stream
	pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
	// Find the decoder for the video stream
	pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
	if(pCodec==NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}
	// Copy context
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}

	// Open codec
	if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
		return -1; // Could not open codec

	// Allocate video frame
	pFrame=av_frame_alloc();

	// Allocate an AVFrame structure
	pFrameRGB=av_frame_alloc();
	if(pFrameRGB==NULL)
		return -1;

	// Determine required buffer size and allocate buffer
	numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
				  pCodecCtx->height);

	printf("+++++++++++++++++++++++++ %d ++++++++++++++++++++++++++\n", numBytes);
	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
		 pCodecCtx->width, pCodecCtx->height);

	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(pCodecCtx->width,
			   pCodecCtx->height,
			   pCodecCtx->pix_fmt,
			   pCodecCtx->width,
			   pCodecCtx->height,
			   AV_PIX_FMT_RGB24,
			   SWS_BICUBIC,
			   NULL,
			   NULL,
			   NULL
			   );
	
	// Read frames and save first five frames to disk
	printf("1/frame rate is %f \n", av_q2d(pFormatCtx->streams[videoStream]->codec->time_base));
	i=0;
	while(av_read_frame(pFormatCtx, &packet)>=0) {
	// Is this a packet from the video stream?
		if(packet.stream_index==videoStream) {
			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if(frameFinished) {
				// Convert the image from its native format to RGB
				sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				// Save the frame to disk
				if (++i || 1)
					SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);

			}

			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}
	}
	// Free the RGB image
	av_free(buffer);
	av_frame_free(&pFrameRGB);

	// Free the YUV frame
	av_frame_free(&pFrame);

	// Close the codecs
	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&pFormatCtx);

	quit = true;
	pthread_join(t1, NULL);

	return 0;
}


void* detector_thread(void* arg)
{
	Mat image;
	ImgData::image_header header;

	while(quit == false) {
		data.GetData(image, header);

		//cout << "seq is " << header.seq << endl;
		if (header.seq >= 0) {
			//cout << "seq is " << header.seq << endl;
			net->Detect(image, header);

		}
		else {
			usleep(10000);
		}
	}
}
