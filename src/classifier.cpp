#include "classifier.h"

#include "ImageProcUtils.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#include<iostream>

using namespace std;
using namespace placeholders;
using namespace cv;

static int terminate1 = 0;

image_proc_utils::Classifier::Classifier(ImageProvider* _provider, AbsDetector* _detectior, bool _visualize) 
    : provider(_provider)
    , detector(_detectior)
    , visualize(_visualize)  
    {

#ifdef _WIN32 
    if (!InitializeCriticalSectionAndSpinCount(&criticalSection, 0x00000400)) {
        THROW_EX(RobotException, "Error in initializing criticalSection");
    }
#else
    if (pthread_mutex_init(&lock, NULL) != 0) {
        THROW_EX(RobotException, "Error in initializing mutex");
    }        
#endif 
    

}

image_proc_utils::Classifier::~Classifier() {
#ifdef _WIN32
    DeleteCriticalSection(&criticalSection);
#else
    pthread_mutex_destroy(&lock);
#endif 

    

    
    provider->Finalize();
    

}

void image_proc_utils::Classifier::Apply(cv::Mat& image, const vector<DetectionInfo>& detections) {
    for (auto& detection : detections) {
        if (detection->detectionClass < DetectionInfo::MAX_CLASS) {
            int x = detection->left;
            int y = detection->top;

            int width = detection->right - detection->left;
            int height = detection->bottom - detection->top;

            rectangle(image, Rect(x, y, width, height), cv::Scalar(0, 255, 0));
        }

        
    }
}


void image_proc_utils::Classifier::GetInfo(int robotIndex, robot_info_t& info) {

    if (robotIndex < 0 || robotIndex >= NO_OF_ROBOTS) {
        THROW_EX(RobotException, "Invalid index");
    }

    bool flag = true;

#ifdef _WIN32
    EnterCriticalSection(&criticalSection);
#else 
    pthread_mutex_lock(&lock);
#endif    

    if (robot_info[robotIndex].header.size() == 0) {
        info.header.resize(0);
    }
    else {
        info.image = robot_info[robotIndex].image;
        info.header = robot_info[robotIndex].header;
    }

#ifdef _WIN32
    LeaveCriticalSection(&criticalSection);
    
#else
    pthread_mutex_unlock(&lock);
#endif

}

void image_proc_utils::Classifier::SetInfo(const vector<char>& image, const vector<char>& header) {
    
    if (header.size() == 0) {
        //cout << "header size is zero" << endl;
        return;
    }

    //printf("last two chars are %x and %x \n", *(image.end()-2) & 0xff, *(image.end()-1) & 0xff);
        
    const ImgData::image_header* p = (ImgData::image_header*)&header[0];
    int robotIndex = p->nRobotNum;
    if (robotIndex < 0 || robotIndex >= NO_OF_ROBOTS) {
        THROW_EX(RobotException, "Invalid Index");
    }
         
#ifdef _WIN32
    EnterCriticalSection(&criticalSection);
#else 
    pthread_mutex_lock(&lock);
#endif    


    robot_info[robotIndex].image = image;
    robot_info[robotIndex].header = header;

#ifdef _WIN32
    LeaveCriticalSection(&criticalSection);
#else
    pthread_mutex_unlock(&lock);
#endif

}

void image_proc_utils::Classifier::Test(const std::vector<char>& k, int l) {
    cout << "size is: " << k.size() << endl;
}


void image_proc_utils::Classifier::Run(Callback callback){

    
    provider->Initialize(std::bind(&Classifier::SetInfo, this, _1, _2));
    
    
    robot_info_t info;
    int robotIndex = 0;
    int count = 0;
    const int duration_ms = 100;
    while (1) {
        GetInfo(robotIndex, info);
        if (info.header.size() == 0) {
            cout << "No data" << endl;
#ifdef _WIN32
            Sleep(duration_ms);
#else       
            usleep(duration_ms*1000);
#endif
            
        }
        else {
            //cout << "Data found \n";
            

            Mat rawData(1, info.image.size(), CV_8UC1, (void*)&info.image[0]);
            Mat decodedImage = imdecode(rawData, IMREAD_COLOR);
            if (decodedImage.data == NULL)
            {
                THROW_EX(RobotException, "Error in decoding image");
            }

            if (visualize == false)
                detector->Detect(&info.image[0], info.image.size(), &info.header[0], info.header.size());
            else {
                vector<DetectionInfo> detections;

                detector->Detect(&info.image[0], info.image.size(), &info.header[0], info.header.size(), detections);

            
                for (auto& a : detections) {
                    a.Apply(decodedImage);
                    
                }
                //Apply(decodedImage, detections);

                
                imshow("Display window", decodedImage);
                int k = waitKey(10); // Wait for a keystroke in the window
                if (k == 27)
                {
                    break;
                }

            }

            
            
            

        }

        robotIndex = (robotIndex + 1) % NO_OF_ROBOTS;

        if (callback != NULL && callback() == false) 
            break;

        if (provider->IsInitialized() == false) {
            THROW_EX(RobotException, "Error in provider !!!");
        }
        const int total = 500000*2;
        //cout << "more to ... " << total-count << endl;
        //if (count++ >= total)
          //  break;
    }

    provider->Finalize();
    
}

