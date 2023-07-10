

#include "..\file_reader.h"


#include "..\detector.h"
#include "..\utils.h"
#include "DetectionInfo.h"

#include "tensorflow/k2tf_projects/water_detector/main/common.h"
//#include "water_detector_RNN.h"
//#include "sfsh_detector.h"


#include "opencv2/highgui.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/imgproc.hpp"

#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>


using namespace std;
using namespace cv;
using namespace k2tf_detector;
using namespace image_proc_utils;

static unique_ptr<AbsDetector> net;
static unique_ptr<FileReader> fileReader;

static clock_t start_time, end_time;

void StartClock();
void StopClock(bool show_elapsed_time);


void DrawRects(const vector<DetectionInfo>& detections, cv::Mat& image, int length=-1);
bool Show(const string& filename, vector<DetectionInfo>& detections);

int detector_test() {

    ServiceContainer* serviceContinaer = ServiceContainer::Instance();
    try {
        bool useDetectors[DetectionInfo::MAX_CLASS] = {0, };
        useDetectors[DetectionInfo::SPARK] = true;
        useDetectors[DetectionInfo::FIRE] = true;
        useDetectors[DetectionInfo::HUMAN] = true;
        useDetectors[DetectionInfo::SMOKE] = true;
        useDetectors[DetectionInfo::FLOOD] = true;

        DetectionSender* sender = serviceContinaer->get<DetectionSender>();
        if (sender == NULL) {
            THROW_EX(RobotException, "Error !!!");
        }
        net = std::make_unique<Detector>(useDetectors, sender);


        //fileReader = make_unique<FileReader>("/home/hasan/raw_data/spark_fire_human_detector/images/test2/jpg", 20, 60);
        //fileReader = make_unique<FileReader>("/home/hasan/raw_data/water_detector/jpegs", 312, 113);
       //fileReader = make_unique<FileReader>("/home/hasan/raw_data/water_detector/pos_test", 60, 369);
        //fileReader = make_unique<FileReader>("/home/hasan/raw_data/water_detector/test_cases", 80, 72);
        fileReader = make_unique<FileReader>("D:\\raw_data\\hsf_detector\\jpegs", 200, 0);
        //fileReader = make_unique<FileReader>("/home/hasan/MyProjects/DS_ImageProcessing/FireSparkHumanDetector/temp2", 1, 5);


        int count = fileReader->GetSize();

        ImgData::image_header header;

        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
        cout << "size is " << count << endl;
        
        


        for (int i=0; i<count; ++i) {
            StartClock();
            string& jpeg_encoding = fileReader->GetEncoding(i);
            string filename = fileReader->GetFilename(i);
            int size = jpeg_encoding.size();
            header.imageSize = size;
            header.seq = 0;
            header.nRobotNum = 0;
            vector<DetectionInfo> detections;
            net->Detect((const char *)&jpeg_encoding[0], size, (const char *) &header, (int) sizeof(header), detections);
            cout << "Total detected: " << detections.size()-1 << endl;
            for (auto& a : detections) {
                if (a->detectionClass != DetectionInfo::MAX_CLASS)
                    cout << a->detectionClass << ":" <<  a.classes[a->detectionClass] << "(" << a->confidence << ")" << endl;
            }
            cout << "------------------------------------------------\n";

            bool next = Show(filename, detections);
            if ( next == false)
                break;

            cout << "filename is " << filename << endl;
            StopClock(true);

        }
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }




    return 0;
}



bool Show(const string& filename, vector<DetectionInfo>& detections) {
    cv::Mat img = imread(filename, IMREAD_COLOR);
    bool flag = true;
    while(1) {
        if(img.empty())
        {
            std::cout << "Could not read the image: " << filename << std::endl;
            return false;
        }

        DrawRects(detections, img);

        imshow("Display window", img);
        int k = waitKey(10); // Wait for a keystroke in the window
        if(k == 27 )
        {   flag = false;
            break;
        }
        break;
        
    }

    return flag;
}

void DrawRects(const vector<DetectionInfo>& detections, cv::Mat& image, int length) {
	for (const auto & detection : detections) {
        if (detection->detectionClass != DetectionInfo::MAX_CLASS) {
            cv::Rect rect(detection->left, detection->top, detection->right-detection->left, detection->bottom-detection-> top);
            rectangle(image, rect, cv::Scalar(0, 255, 0));
            //cout << detection.classes[detection->detectionClass] << endl;
        }
	}
}




void StartClock() {
  start_time = clock();
}

void StopClock(bool show_elapsed_time) {

  end_time = clock();
  auto elapsed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  if (show_elapsed_time)
    cout << "elapsed time: " << elapsed << endl;
}

