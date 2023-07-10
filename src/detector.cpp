/*
 * darknet.cpp
 *
 *  Created on: Aug 28, 2020
 *      Author: hasan
 */

#include "detector.h"
#include "utils.h"



//#include "wfss_detector.h"
#include "yolov3_generic.h"


#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;
//using namespace cv;
//using namespace cv::dnn;




using namespace k2tf_detector;



image_proc_utils::Detector::Detector(AbsDetectionSender* detectionSender, const bool _useDetecors[DetectionInfo::MAX_CLASS]) : AbsDetector(detectionSender)

{

	for (int i=0; i<DetectionInfo::MAX_CLASS; ++i) {
		useDetecors[i] = _useDetecors[i];
	}	

	countingThresholds[DetectionInfo::FLOOD] = 3;
	countingThresholds[DetectionInfo::SMOKE] = 5;

	useDetecorGroups[WFSS_DETECTOR] = true;
	useDetecorGroups[PERSON_DETECTOR] = true;

	Init_K2TF_Detectors();




}




void image_proc_utils::Detector::Init_K2TF_Detectors() {

	ServiceContainer* serviceContinaer = ServiceContainer::Instance();
	string model_path = string(getenv("TF_ROOT")) + "/tensorflow/k2tf_projects/water_detector/models";

  	if (useDetecorGroups[WFSS_DETECTOR]) {
		string model_name = "model_wfss";

		auto yolov3 = model_path + "/" + "yolov3_wfss.pb";
		vector<string> yolov3_inputs = {"input_1"};
		vector<string> yolov3_outputs = {"conv_81/BiasAdd", "conv_93/BiasAdd", "conv_105/BiasAdd"};
		model_ext_t yolov3_model(yolov3, yolov3_inputs, yolov3_outputs);

		map<int,int> classmap = {{0, DetectionInfo::FLOOD}, {1, -1}, {2, DetectionInfo::FIRE}, {3, DetectionInfo::SPARK}, {4, DetectionInfo::SMOKE}};		// 1 is for bg class
		
		wpDetectors[WFSS_DETECTOR] = std::make_unique<YoloV3_Generic>(model_name.c_str(), JPEG_ENCODING, yolov3_model,classmap);

	}


	if (useDetecorGroups[PERSON_DETECTOR]) {
		string model_name = "model_person";

		auto yolov3 = model_path + "/" + "yolov3_coco.pb";
		vector<string> yolov3_inputs = {"input_1"};
		vector<string> yolov3_outputs = {"conv_81/BiasAdd", "conv_93/BiasAdd", "conv_105/BiasAdd"};
		model_ext_t yolov3_model(yolov3, yolov3_inputs, yolov3_outputs);

		map<int,int> classmap = {{0, DetectionInfo::HUMAN}};
		
		wpDetectors[PERSON_DETECTOR] = std::make_unique<YoloV3_Generic>(model_name.c_str(), JPEG_ENCODING, yolov3_model,classmap);

	}
		

}



vector<image_proc_utils::DetectionInfo> image_proc_utils::Detector::FilterResults(const vector<detection_results_t>& inputs, int detectorGroupId) {

    vector<DetectionInfo> outputs;

	if (detectorGroupId >= MAX_DETECTOR)
		return outputs;

	if (useDetecorGroups[detectorGroupId] == false)
		return outputs;

    int num_classes = wpDetectors[detectorGroupId]->GetNumOfClasses();

	map<int, int> classmap;

    cout << "grp id " << detectorGroupId << " num clssses " << num_classes << endl;
	wpDetectors[detectorGroupId]->GetClassmap(classmap);

    for (int k=0; k<num_classes; ++k) {

        vector<float> prob_array;

        vector<detection_results_t> temp;
        vector<size_t> pick;
		int classId = classmap[k];
		

        if (classId < 0)
            continue;

        for (auto& a : inputs) {
            if (a.classId == classId) {
                temp.push_back(a);
                prob_array.push_back(a.pred);
            }
        }

        if (temp.size() == 0)
            continue;

		
		vector<DetectionInfo> cls_outputs;
		
        YoloV3_Generic::non_max_suppression(temp, prob_array, pick, nmsThresholds[classId]);
        

        for (auto& a : pick) {
            auto filtered_result = temp[a];

            cout << "pred: " << filtered_result.pred << endl;

			if (useDetecors[classId] && filtered_result.pred > confThresholds[classId]) {
				
				auto& res = filtered_result;
				
                DetectionInfo info((DetectionInfo::class_t)(classId), res.y, res.x, res.y+res.h, res.x+res.w, res.pred, header.seq, header.nRobotNum);
                cls_outputs.push_back(info);
				

			}

        }

		if (cls_outputs.size() > countingThresholds[classId]) {
			outputs.insert(outputs.end(), cls_outputs.begin(), cls_outputs.end());
		}


        

    }

    return outputs;
}

void print_resuts(vector<detection_results_t>& results) {
    for (int i=0; i<results.size(); ++i) {
            detection_results_t& res = results[i];
            printf("object: %d, coord: upper_left: (%5d, %5d) lower_right: (%5d, %5d) \t prob: %6.8f \n", res.classId, res.x, res.y, res.x+res.w, res.y+res.h, res.pred);
    }

    printf("total detected %5d\n", results.size());
}

void image_proc_utils::Detector::Detect(const char * data, int dataSize, const char * info, int infoSize, vector<image_proc_utils::DetectionInfo> & detections) {

	
	if (infoSize < sizeof(header)) {
		THROW_EX(RobotException, "invalid info size");
	}
	
	cout << "***************************1" << endl;

	memcpy((void *)&header, &info[0], sizeof(header));

	if (dataSize < header.imageSize) {
		THROW_EX(RobotException, "invalid data size");
	}

	std::vector<detection_results_t> results;


	for (int i=0; i<MAX_DETECTOR; ++i) {

		if (useDetecorGroups[i] == false)
			continue;

		string encoding;
		encoding.resize(dataSize);
		memcpy(&encoding[0], data, dataSize);

		wpDetectors[i]->Detect(encoding, results);
		//print_resuts(results);
		auto info = FilterResults(results, i);
		detections.insert(detections.end(), info.begin(), info.end());
		
	}
	//cout << "detections.size()=" << detections.size() << endl;

	image_proc_utils::DetectionInfo triggerer(DetectionInfo::MAX_CLASS, 0, 0, 0, 0, 0.0, header.seq, header.nRobotNum);
	
	detections.push_back(triggerer);

}
