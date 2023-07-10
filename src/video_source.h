#ifndef _VIDEO_SOURCE_H
#define _VIDEO_SOURCE_H

#include "ImageSource.h"
#include "ImageProcUtils.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#include <vector>

namespace image_proc_utils {


class VideoSource : public ImageSource {
 public:
    VideoSource(const std::string& root, float start=0.0, float end=1.0);
    void Reset();
    void GetNext(std::vector<char>& image, std::vector<char>& header);

 private:
    std::unique_ptr<cv::VideoCapture> cap;
    int frameCounter;
    const int quality = 94;
    const int epsilon = 0.001;
    int startIndex;
    int endIndex;
    __int64 seq;

};

}
#endif
