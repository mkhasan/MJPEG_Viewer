/*
 * darknet.h
 *
 *  Created on: Aug 28, 2020
 *      Author: hasan
 */

#ifndef SRC_DETECTOR_H_
#define SRC_DETECTOR_H_

#include "abs_detector.h"


//#include "tensorflow/k2tf_projects/water_detector/main/common.h"

#include <vector>
#include <memory>



namespace image_proc_utils {

class Detector : public AbsDetector {
	enum {WFSS_DETECTOR, PERSON_DETECTOR, MAX_DETECTOR};

	const float confThresholds[DetectionInfo::MAX_CLASS]= {0.90, 0.8, 0.5, 0.6, 0.6}; // individual threshold
	const float nmsThresholds[DetectionInfo::MAX_CLASS] = {0.5, 0.5, 0.5, 0.25, 0.5};  // Non-maximum suppression threshold
	int countingThresholds[DetectionInfo::MAX_CLASS] = { 0, };

	bool useDetecors[DetectionInfo::MAX_CLASS];
	bool useDetecorGroups[MAX_DETECTOR];
	
	


	std::unique_ptr<k2tf_detector::K2TF_Detector> wpDetectors[MAX_DETECTOR];	// wfss and person detectors

	void Init_K2TF_Detectors();
	
	


public:

	Detector(AbsDetectionSender* detectionSender, const bool useDetecors[DetectionInfo::MAX_CLASS]);
	std::vector<DetectionInfo> FilterResults(const std::vector<k2tf_detector::detection_results_t>& inputs, int detectorGroupId);
	virtual void Detect(const char * data, int dataSize, const char * info, int infoSize, std::vector<DetectionInfo>& detections) override;
};

}


#endif /* SRC_DARKNET_H_ */
