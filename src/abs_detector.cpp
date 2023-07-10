/*
 * detector.cpp
 *
 *  Created on: Aug 28, 2020
 *      Author: hasan
 */

#include "abs_detector.h"
#include "ImageProcUtils.h"
#include "utils.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>


using namespace std;
using namespace cv;


image_proc_utils::AbsDetector::AbsDetector(AbsDetectionSender* _detectionSender) : detectionSender(_detectionSender)
{
	//const std::string HOST = "192.168.100.87";
	//const int PORT = 1883;
	// where netComm is true detection results will be sent to the client program
	// however for debugging we may not want that..netComm must be false at that time
	/*
	if (netComm) {
		image_proc_utils::ServiceContainer * serviceContainer = image_proc_utils::ServiceContainer::Instance();
		detectionSender = serviceContainer->get<DETECTION_SENDER>();
	}
	*/



}

image_proc_utils::AbsDetector::~AbsDetector() {

}


void image_proc_utils::AbsDetector::Detect(const std::vector<char>& data, const std::vector<char>& info) {

	Detect(&data[0], data.size(), &info[0], info.size());
}


void image_proc_utils::AbsDetector::Detect(const char * data, int dataSize, const char * info, int infoSize) {

	std::vector<DetectionInfo> detections;
	Detect(data, dataSize, info, infoSize, detections);		// get detection results in the form of DetectionInfo class from proper implementation
	if (detections.size() > 0)								// this abstract class simple sends the result towards network if netComm is true
		SendMessage(detections);							// this way the implementer do not have to think how or where to send the result
															// only focus will be to get good detection result

}


void image_proc_utils::AbsDetector::SendMessage(const std::vector<DetectionInfo>& detections) {

	if (detectionSender != NULL)
		detectionSender->SendMessage(detections);

}


