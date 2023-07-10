

#include "opencv_ext.h"

#include <time.h>
#include <iostream>
#include <string>



using namespace std;
using namespace cv;
using namespace k2tf_detector;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

string preprocessed_imgage_path = "/home/hasan/MyProjects/DS_ImageProcessing/SFSH_keras/pipe.ext"; 
string arg_sort_path = "/home/hasan/programs/MyProjects/DS_ImageProcessing/WaterDetectorRNN/arg_sort.ext";

void get_data(float* ch1, float* ch2, float* ch3, size_t * arg_list, int& size) {

	cv::FileStorage fs_read = cv::FileStorage(preprocessed_imgage_path, cv::FileStorage::READ);

	cv::Mat img;
	fs_read["floatdata"] >> img;

  fs_read.release();

  /*
  cv::Mat arg_sort;

  fs_read = cv::FileStorage(arg_sort_path, cv::FileStorage::READ);
  fs_read["size_t_data"] >> arg_sort;

  fs_read.release();
  */

  //cout << type2str(arg_sort.type()) << endl;

  //cout << arg_sort.rows << " " << arg_sort.cols << endl;
  /*
  for (int i=0; i<arg_sort.rows; ++i) {
    arg_list[i] = (size_t) arg_sort.at<int>(i, 0);
  }
  */

  //size = arg_sort.rows;
  
  //cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  
	//cout << img.cols << endl;

  int rows = img.rows;
  int cols = img.cols;
  size = rows*cols;
  
  for (int i=0; i<rows; ++i) {
    for (int j=0; j<cols; ++j) {
      cv::Vec3f elem;
      elem = img.at<Vec3f>(i,j);
      ch1[i*cols+j] = elem[0];
      ch2[i*cols+j] = elem[1];
      ch3[i*cols+j] = elem[2];
    }

  }

  //size = rows*cols;

	string str = type2str(img.type());
	//cout << str << endl;


}

void Show(const string& filename, vector<detection_results_t>& results) {
    cv::Mat img = imread(filename, IMREAD_COLOR);
    while(1) {
        if(img.empty())
        {
            std::cout << "Could not read the image: " << filename << std::endl;
            return;
        }

        DrawRects(results, img);

        imshow("Display window", img);
        int k = waitKey(10); // Wait for a keystroke in the window
        if(k == 27)
        {
            break;
        }
        imwrite("temp.png", img);
    }
}

void DrawRects(const vector<detection_results_t>& results, cv::Mat& image, int length) {
	for (const auto & result : results) {
		cv::Rect rect(result.x, result.y, result.w, result.h);
		rectangle(image, rect, cv::Scalar(0, 255, 0));
	}
}

static clock_t start_time, end_time;

void StartClock() {
  start_time = clock();
}

void StopClock(bool show_elapsed_time) {

  end_time = clock();
  auto elapsed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  if (show_elapsed_time)
    cout << "elapsed time: " << elapsed << endl;
}

