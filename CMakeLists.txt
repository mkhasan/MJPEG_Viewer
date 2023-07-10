	
cmake_minimum_required(VERSION 3.10.2)

project(rtsp_client)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Enable C++11

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ggdb -Wall -std=c++17") 
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -DBSD=1")

 
add_definitions(-DBIN_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" ) 
add_definitions(-DDETECTION_SENDER=FakeDetectionSender)
add_definitions(-DFAKE_DETECT=0) 
add_definitions(-D__int64=__int64_t)


message("runtime dir is ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}") 
#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp")


find_package (OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(

./live555-master/UsageEnvironment/include
./live555-master/groupsock/include
./live555-master/liveMedia/include
./live555-master/BasicUsageEnvironment/include
./src
$ENV{IMAGE_PROC_UTILS_ROOT}
#$ENV{FFMPEG_ROOT}
$ENV{TF_ROOT}
#include_directories ($ENV{TF_ROOT}/tensorflow/k2tf_projects/water_detector/main)
#include_directories ($ENV{TF_ROOT}/tensorflow/k2tf_projects/smoke_detector/main)

#include_directories($ENV{TF_ROOT}/tensorflow/k2tf_projects/water_detector/main)
include_directories($ENV{TF_ROOT}/tensorflow/k2tf_projects/wfss_detector/main)
#include_directories($ENV{TF_ROOT}/tensorflow/k2tf_projects/sfsh_detector/main)
include_directories($ENV{TF_ROOT}/tensorflow/k2tf_projects/yolov3_generic/main)

include_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/include)
include_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/include/src)

include_directories($ENV{TF_ROOT})

)


#find_library(AVFORMAT SHARED avformat HINTS $ENV{FFMPEG_ROOT}/libavformat)
#find_library(AVFILTER SHARED avfilter HINTS $ENV{FFMPEG_ROOT}/libavfilter)
#find_library(AVUTIL SHARED avutil HINTS $ENV{FFMPEG_ROOT}/libavutil)
#find_library(AVDEVICE SHARED avdevice HINTS $ENV{FFMPEG_ROOT}/libavdevice)
#find_library(AVCODEC SHARED avcodec HINTS $ENV{FFMPEG_ROOT}/libavcodec)
#find_library(SWRESAMPLE SHARED swresample HINTS $ENV{FFMPEG_ROOT}/libswresample)
#find_library(SWSCALE SHARED swscale HINTS $ENV{FFMPEG_ROOT}/libswscale)



#message("avformat is ${AVFORMAT}")
#message("avfilter is ${AVFILTER}")
#message("avutil is ${AVUTIL}")
#message("avdevice is ${AVDEVICE}")
#message("avcodec is ${AVCODEC}")
#message("swscale is ${SWSCALE}")


#add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
#target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

find_library(MQTTPP mosquittopp)

message("mqtt_dir dir is ${MQTTPP}")

link_directories(

./live555-master/liveMedia
./live555-master/groupsock
./live555-master/BasicUsageEnvironment
./live555-master/UsageEnvironment
)

#link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/water_detector/main)
link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/smoke_detector/main)
#link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow)

link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/water_detector/main)
link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/water_detector_RNN/main)
link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/wfss_detector/main)
link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/sfsh_detector/main)
link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow/k2tf_projects/yolov3_generic/main)

link_directories($ENV{TF_ROOT}/bazel-bin/tensorflow)

message(${OpenCV_INCLUDE_DIRS})

#add_executable(${PROJECT_NAME} src/image_processor.cpp src/image_provider_net.cpp src/abs_detection_sender.cpp src/classifier.cpp src/utils.cpp src/abs_detector.cpp src/fake_detector.cpp 
#    src/image_provider_shm.cpp
#    $ENV{IMAGE_PROC_UTILS_ROOT}/FakeSource.cpp 
#    $ENV{IMAGE_PROC_UTILS_ROOT}/ImageSource.cpp
#    $ENV{IMAGE_PROC_UTILS_ROOT}/DetectionInfo.cpp
#    src/video_source.cpp 
#    src/detector.cpp
   
#)

#target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} liveMedia groupsock BasicUsageEnvironment UsageEnvironment yolov3_generic water_detector water_detector_RNN sfsh_detector tensorflow_cc tensorflow_framework pthread)

add_executable(${PROJECT_NAME} src/client.cpp src/abs_detector.cpp  src/fake_detector.cpp src/abs_detection_sender.cpp src/utils.cpp src/impl/unix.cc src/custom_detection_sender.cpp)
target_link_libraries(${PROJECT_NAME} liveMedia groupsock BasicUsageEnvironment UsageEnvironment ${OpenCV_LIBS} pthread)

#add_executable(test src/test.cpp)

#add_executable(yolov3_coco_test src/yolov3_coco_test src/file_reader.cpp src/opencv_ext.cpp)
#target_link_libraries(yolov3_coco_test yolov3_coco sfsh_detector wfss_detector water_detector tensorflow_cc ${OpenCV_LIBS} stdc++fs pthread)


