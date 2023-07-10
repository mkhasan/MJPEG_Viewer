#ifndef _CLASSIFIER_H 
#define _CLASSIFIER_H 

#include "image_provider.h"
#include "abs_detector.h"

#include "Config.h"

#include "opencv2/highgui.hpp"

#include "opencv2/imgproc.hpp"


#ifdef _WIN32
#include "Windows.h"
#else
#include<pthread.h>
#endif


#include <vector>
#include <functional>

namespace image_proc_utils {
	class Classifier {

	private:
		typedef struct {
			std::vector<char> image;
			std::vector<char> header;
		} robot_info_t;

		

	private:
		robot_info_t robot_info[NO_OF_ROBOTS];

		
		
		ImageProvider* provider;
		AbsDetector* detector;

        bool visualize;



	public:
		typedef std::function<bool(void)> Callback;

		Classifier(ImageProvider* provider, AbsDetector *detector, bool visualize=false);
		~Classifier();
		
		void GetInfo(int robotIndex, robot_info_t& info);
		void SetInfo(const std::vector<char>& image, const std::vector<char>& header);

		void Test(const std::vector<char>& k, int l);
		void Run(Callback callback);

#ifdef _WIN32
		CRITICAL_SECTION criticalSection;
#else
    pthread_mutex_t lock;

#endif 


		static void Apply(cv::Mat& image, const std::vector<DetectionInfo>& detections);




	};

}

#endif 

