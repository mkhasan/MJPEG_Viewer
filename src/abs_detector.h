/*
 * detector.h
 *
 *  Created on: Aug 28, 2020
 *      Author: hasan
 */

#ifndef ABS_DETECTOR_H_
#define ABS_DETECTOR_H_



#include "DetectionInfo.h"
#include "abs_detection_sender.h"

#include <vector>

namespace image_proc_utils {



class AbsDetector {

	bool netComm;

public:

	AbsDetector(AbsDetectionSender* detectionSender);
	virtual ~AbsDetector();

	void Detect(const std::vector<char>& data, const std::vector<char>& info);
	virtual void Detect(const char * data, int dataSize, const char * info, int infoSize);
	virtual void Detect(const char * data, int dataSize, const char * info, int infoSize, std::vector<DetectionInfo>& detections) = 0;


protected:

	ImgData::image_header header;

	AbsDetectionSender* detectionSender;

	void SendMessage(const std::vector<DetectionInfo>& detections);
};

class FakeDetector : public AbsDetector {
public:
	FakeDetector(AbsDetectionSender* detectionSender, const bool *temp=NULL);
	virtual void Detect(const char* data, int dataSize, const char* info, int infoSize, std::vector<DetectionInfo>& detections) override;
};

}

#endif /* SRC_DETECTOR_H_ */
