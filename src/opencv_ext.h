#ifndef _OPENCV_EXT_H
#define _OPENCV_EXT_H

#include "tensorflow/k2tf_projects/water_detector/main/common.h"

#include "opencv2/highgui.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/imgproc.hpp"


void Show(const std::string& filename, std::vector<k2tf_detector::detection_results_t>& results);
void DrawRects(const std::vector<k2tf_detector::detection_results_t>& results, cv::Mat& image, int length=-1);
void get_data(float* ch1, float* ch2, float* ch3, size_t * arg_list, int& size);

void StartClock();
void StopClock(bool show_elapsed_time);

#endif