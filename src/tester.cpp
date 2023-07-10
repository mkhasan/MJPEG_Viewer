/*
 * detection_class_test.cpp
 *
 *  Created on: Aug 28, 2020
 *      Author: hasan
 */


#include "ImageProcUtils.h"
#include "Config.h"

#include "darknet.h"
#include <iostream>
#include <fstream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/imgproc.hpp"
#include <signal.h>
#include <memory>


using namespace std;
using namespace cv;

using namespace image_proc_utils;

static bool isRunning = true;


static void my_handler(int s){
	printf("[Ctrl]+C\n");
	isRunning = false;

}


static int get_filesize(string filename) {
	streampos begin, end;
	ifstream myfile(filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	myfile.close();
	return end - begin;
}

static void get_data(char * data, int & len, string filename) {


	len = -1;

	ifstream myfile(filename, ios::binary);
	if (myfile.is_open()) {
		int size = get_filesize(filename); // myfile.tellg();

		myfile.seekg(0, ios::beg);
		myfile.read(data, size);
		len = size;

	}

}

#include "custom_detection_sender.h"


int main(int argc, char *argv[]) {
	try {

		const int seq = 3;
		vector<image_proc_utils::DetectionInfo> detections;
		detections.push_back(image_proc_utils::DetectionInfo(DetectionInfo::HUMAN, 14, 622, 633, 876, 0.99, seq, 1));
		detections.push_back(image_proc_utils::DetectionInfo(DetectionInfo::HUMAN, 114, 122, 533, 676, 0.92, seq, 1));
		detections.push_back(image_proc_utils::DetectionInfo(DetectionInfo::HUMAN, 214, 222, 633, 376, 0.91, seq, 1));
		image_proc_utils::DetectionInfo triggerer(DetectionInfo::MAX_CLASS, 0, 0, 0, 0, 0.0, seq, -1);
		detections.push_back(triggerer);



		DetectionSender * sender = new CustomDetectionSender("192.168.100.87", 80);//DETECTION_READER_CUSTOM_PORT);
//		DetectionSender * sender = new MqttDetectionSender("192.168.100.87", 1883);


		sender->SendMessage(detections);

		delete sender;
	}
	catch (const std::exception & exp) {
		cout << "Error: " << exp.what() << endl;
	}
}

static int main1(int argc, char *argv[]) {

	 struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);



	try {
		int index = 15;
		if (argc > 1) {
			sscanf(argv[1], "%d", &index);
		}

		static char data[ImgData::WIDTH * ImgData::HEIGHT * ImgData::CHANNEL + 1024];
		char image_path[256];

		std::unique_ptr<Detector> net = std::make_unique<DarkNet>();

		

		cout << "Enter any key" << endl;
		char ch;
		cin >> ch;

		//index = 0;
		for (int i = 0; i < 100 && isRunning; i++, index++) {

			static char data[ImgData::WIDTH * ImgData::HEIGHT * ImgData::CHANNEL + 1024];
			char image_path[256];// = "E:\\MyProjects\\VOC2012\\JPEGImages";

			sprintf(image_path, "%s/%s/%06d.jpg", BIN_DIR, "img_path", index);
			cout << image_path << endl;
			int len;
			get_data(data, len, image_path);

			ImgData::image_header header;
			header.imageSize = len;
			header.seq = index;
			header.nRobotNum = 2;

			net->Detect(&data[0], len, (char *)&header, (int)sizeof(header));


		}

		

		


	}

	catch (const exception & e) {
		cout << string(e.what()) << endl;
	}

	return 0;
}

