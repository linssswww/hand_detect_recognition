//
// Created by zzz on 2016/9/20.
//

#ifndef HAND_DETECT_RECOGNITION_HAND_DETECT_H
#define HAND_DETECT_RECOGNITION_HAND_DETECT_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "hand_recognition.h"
#include <opencv2/opencv.hpp>
using namespace cv;
class hand_detect {

    public:
        hand_detect();
        ~hand_detect();
        void get_frame(Mat frame);
        void meanshift_init();
    public:
        bool flag;
    private:
        void get_roi();
        void get_init_pt();
        void get_histogram();
        void pre_deal();
        void track_hand();
        void get_handroi(Mat &f);
        void back_ground(Mat frame);
        Mat detect_by_color(Mat frame);
        void reduce_minArea(Mat &frame);
private:
        Mat roi;
        Mat maskroi;
        Mat hist;
        Rect init_pt;
        Rect hand_region;
        hand_recognition *he;
        Mat *img;
        BackgroundSubtractorMOG2 mog;//背景差
        Mat frame;
};


#endif //HAND_DETECT_RECOGNITION_HAND_DETECT_H
