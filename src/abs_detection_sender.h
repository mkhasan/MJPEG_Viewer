/*
 * DetectionSender.h
 *
 *  Created on: Sep 7, 2020
 *      Author: hasan
 */

#ifndef _ABS_DETECTIONSENDER_H
#define _ABS_DETECTIONSENDER_H

#include "DetectionInfo.h"

#include <string>
#include <vector>

namespace image_proc_utils {

class AbsDetectionSender {
public:
	AbsDetectionSender(const std::string & hostname, int port);
	virtual ~AbsDetectionSender();

	virtual void SendMessage(const std::vector<DetectionInfo>& detections) = 0;
protected:
	std::string hostname;
	int port;
};

class FakeDetectionSender : public AbsDetectionSender {
public:
	FakeDetectionSender(): AbsDetectionSender("", 0) {
	}

	void SendMessage(const std::vector<DetectionInfo>&info) override {

	}
};

}

#endif /* _ABS_DETECTIONSENDER_H */
