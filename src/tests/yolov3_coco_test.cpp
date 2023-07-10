


#include "file_reader.h"

#include "tensorflow/k2tf_projects/water_detector/main/common.h"
#include "yolov3_coco.h"
//#include "tensorflow/k2tf_projects/water_detector/main/water_detector.h"

#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>

using namespace std;
using namespace k2tf_detector;

#ifdef USE_OPENCV 

#include "opencv_ext.h"
#endif

std::unique_ptr<K2TF_Detector> detector;
std::unique_ptr<FileReader> fileReader;
 

 void print_resuts(vector<detection_results_t>& results) {
    for (int i=0; i<results.size(); ++i) {
            detection_results_t& res = results[i];
            printf("object: %d, coord: upper_left: (%5d, %5d) lower_right: (%5d, %5d) \t prob: %6.8f \n", res.classId, res.x, res.y, res.x+res.w, res.y+res.h, res.pred);
    }

    printf("total detected %5d\n", results.size());
}


int main(int argc, char *argv[]){


	
	//string root = "/home/hasan/MyProjects/DS_ImageProcessing/FireSparkHumanDetector/temp2";
	//string root = "/home/hasan/ssd/processed_data/smoke_detector/jpegs";

	//fileReader = make_unique<FileReader>(root, 1, 56);
	fileReader = make_unique<FileReader>("/home/hasan/MyProjects/DS_ImageProcessing/FireSparkHumanDetector/temp2", 20, 0);

	std::vector<detection_results_t> results;


	string model_path = string(getenv("TF_ROOT")) + "/tensorflow/k2tf_projects/yolov3_coco/models";

	auto yolov3 = model_path + "/" + "yolov3_coco.pb";
	vector<string> yolov3_inputs = {"input_1"};
	vector<string> yolov3_outputs = {"conv_81/BiasAdd", "conv_93/BiasAdd", "conv_105/BiasAdd"};
	map<int, int> classmap;
	classmap[0] = 2;
	model_ext_t yolov3_model(yolov3, yolov3_inputs, yolov3_outputs);


	float* ch1;
	float* ch2;
	float* ch3;
	size_t* arg_list;
	int size = 0;
	#ifdef USE_OPENCV

	
	int rows = 416;
	int cols = 416;
	size = rows*cols;

	ch1 = new float[rows*cols];
	ch2 = new float[rows*cols];
	ch3 = new float[rows*cols];
	arg_list = new size_t[9*37*66];
	get_data(ch1, ch2, ch3, arg_list, size);
	
	#endif
	//detector = std::make_unique<SFSH_Detector>(argv[0], JPEG_ENCODING, yolov3_model, ch1, ch2, ch3, arg_list, size);
	detector = std::make_unique<YOLOV3_COCO>(argv[0], JPEG_ENCODING, yolov3_model, classmap);


	int fileCount = fileReader->GetSize();
	for (int i=0; i<fileCount; ++i) {
	StartClock();
		string& jpeg_encoding = fileReader->GetEncoding(i);
		detector->Detect(jpeg_encoding, results);
#ifdef USE_OPENCV
		string filename = fileReader->GetFilename(i);
		//Show(filename, results);
#endif
        print_resuts(results);
        StopClock(true);
	}


	if (size) {
		delete[] ch1;
		delete[] ch2;
		delete[] ch3;
		delete[] arg_list;
	}


	return 0;
}

