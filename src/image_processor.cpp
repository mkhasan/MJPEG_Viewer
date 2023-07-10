/*
 * image_processor.cpp
 *
 *  Created on: Aug 27, 2020
 *      Author: hasan
 */


#include "image_provider_net.h"

#include <unistd.h>
#include <signal.h>

#include "utils.h"
#include "abs_detector.h"
#include "abs_detection_sender.h"
#include "classifier.h"
#include "FakeSource.h"
#include "image_provider_shm.h"
#include "video_source.h"
#include "detector.h"


#include <vector>
#include <memory>
#include <stdio.h>
#include <iostream>


using namespace std;
using namespace image_proc_utils;

static void my_handler(int s);

static bool quit = false;
bool get_status(void) {
    //cout << "hello" << endl;
    return quit == false;
}


int main(int argc, char * argv[]) {


   	struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);



    //const string root = "/home/hasan/raw_data/hsf_images/jpegs/";
    string root = "/home/hasan/temp/R1_color_20210413_133253.avi";
    if (argc >= 2) {
        root = argv[1];
    }

    /*
    ServiceContainer* serviceContainer = ServiceContainer::Instance();
    try {

        bool useDetectors[DetectionInfo::MAX_CLASS] = { 0, };
        useDetectors[DetectionInfo::SPARK] = true;
        useDetectors[DetectionInfo::FIRE] = true;
        useDetectors[DetectionInfo::HUMAN] = true;
        useDetectors[DetectionInfo::SMOKE] = true;
        useDetectors[DetectionInfo::FLOOD] = false;

    
        //serviceContainer->set<CustomDetectionSender>("127.0.0.1");
        serviceContainer->set<FakeDetectionSender>();
        if (argc >= 2) {
            string root = argv[1];
            serviceContainer->set<FakeSource>(root);
            serviceContainer->set<ImageProviderShm, FakeSource>();
        }
        else {
            serviceContainer->set<VideoSource>("R1_color_20210409_133833.avi", 0.0, 1.0);
            serviceContainer->set<ImageProviderShm, VideoSource>();
        }
        //serviceContainer->set<ShmSource>("D:\\raw_data\\hsf_detector\\jpegs\\");
        
        serviceContainer->set<FakeDetector, FakeDetectionSender>(useDetectors);
        //serviceContainer->set<FakeDetector, FakeDetectionSender>();
        serviceContainer->set<Classifier, ImageProviderShm, FakeDetector>(true);

        Classifier* p = serviceContainer->get<Classifier>();
        p->Run(nullptr);
        //detector_test();
    }
    catch (const ServiceContainerException& exp) {
        //cout << "Error !!!" << exp.what() << endl;
    }

    catch (const exception& exp) {
        cout << exp.what() << endl;
    }


    */
    const bool useDetecors[] = {true, true, true, true, true};        // FIRE, SPARK, HUMAN, SMOKE, FLOOD  

    
    try {


        ServiceContainer* serviceContainer = ServiceContainer::Instance();
        serviceContainer->set<FakeDetectionSender>();
        if (argc >= 2) {

            serviceContainer->set<FakeSource>(argv[1]);
            
            
            serviceContainer->set<ImageProviderShm, FakeSource>();

        }
        else {
            serviceContainer->set<VideoSource>("R1_color_20210414_110209.avi", 0.85, 0.95);
            serviceContainer->set<ImageProviderShm, VideoSource>();

        }

        
        serviceContainer->set<Detector, FakeDetectionSender>(useDetecors);
        serviceContainer->set<Classifier, ImageProviderShm, Detector>(true);


        serviceContainer->get<Classifier>()->Run(get_status);
        
        
        sleep(1);
        

        
    }

    catch(const ServiceContainerException& e) {
        cout << e.message << endl;
    }

    catch (const exception& e) {
        cout << e.what() << endl;
    }



	return 0;
}


void my_handler(int s) {
    printf("Caught signal %d\n",s);

    quit = true;

}
