#include "abs_detector.h"

#include "ImageProcUtils.h"

#include <iostream>

using namespace std;

image_proc_utils::FakeDetector::FakeDetector(AbsDetectionSender* _detectionSender, const bool* temp) : AbsDetector(_detectionSender) {

}

void image_proc_utils::FakeDetector::Detect(const char* data, int dataSize, const char* info, int infoSize, std::vector<DetectionInfo>& detections) {

	int width = 100 + (rand() % 20);
	int height = 80 + rand() % 30;

	int x = 600;
	int y = 300;
	
	int x1 = x - width / 2;
	int y1 = y - height / 2;

	int x2 = x + width / 2;
	int y2 = y + height / 2;

	const ImgData::image_header* p = (ImgData::image_header*) info;
	__int64 seq = p->seq;
	int robotIndex = p->nRobotNum;

	detections.resize(0);
	
	
	detections.push_back(DetectionInfo(DetectionInfo::HUMAN, y1, x1, y2, x2, 0.99, seq, robotIndex));

	DetectionInfo triggerer(DetectionInfo::MAX_CLASS, 0, 0, 0, 0, 0.0, seq, robotIndex);
	detections.push_back(triggerer);
	


}