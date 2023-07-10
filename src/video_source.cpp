
#include "video_source.h"

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;

image_proc_utils::VideoSource::VideoSource(const string& root, float start, float end) : ImageSource(root), frameCounter(0), seq(0) {

    cap = make_unique<VideoCapture>(root);
    if (cap->isOpened() == 0) {
        THROW_EX(RobotException, format("Error in opening file %s", root.c_str()).c_str());
    }    

    startIndex = 0;
    endIndex = cap->get(CAP_PROP_FRAME_COUNT);

    if (start > epsilon) {
        int total = cap->get(CAP_PROP_FRAME_COUNT);
        startIndex = int(float(total)*start);
    }

    if (end < 1-epsilon) {
        int total = cap->get(CAP_PROP_FRAME_COUNT);
        endIndex = int(float(total)*end);
    }

    if (startIndex > endIndex) {
        THROW_EX(RobotException, format("index format error start = %f end = %f", start, end).c_str());
    }

    if (startIndex > 0) {
        Reset();
    }

    cout << "endIndex is " << endIndex << endl;
    cout << "startIndex is " << startIndex << endl;
    //cout << format("total frames: %d\n", cap->get(CAP_PROP_FRAME_COUNT)) << endl;
} 

void image_proc_utils::VideoSource::Reset() {
    
    frameCounter = 0;
    cap->set(CAP_PROP_POS_FRAMES, startIndex);
    cout << "resetting \n";
}


void image_proc_utils::VideoSource::GetNext(vector<char>& image, vector<char>& header) {
    
    Mat frame;
    *cap >> frame;
    frameCounter ++;

    if (frameCounter+startIndex >= endIndex) {
        Reset();
    }
    //cout << frameCounter << "\n";

    vector<uchar> buff;
    vector<int> params(2);
    params[0] = IMWRITE_JPEG_QUALITY;
    params[1] = quality;
    imencode(".jpg", frame, buff, params);
    image.resize(buff.size());
    memcpy(&image[0], &buff[0], buff.size());

    ImgData::image_header h;
    h.nRobotNum = 0;
    h.seq = seq++;
    h.quality = quality;

    header.resize(sizeof(h));
    memcpy((void*) &header[0], (void *)&h, sizeof(h));


    static int index=0;
    if (1) {
        imwrite(format("/tmp/export/%06d.jpg", index++), frame);
    }

}


