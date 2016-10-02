//
// Created by zzz on 2016/9/20.
//

#ifndef HAND_DETECT_RECOGNITION_HAND_EXTRACT_H
#define HAND_DETECT_RECOGNITION_HAND_EXTRACT_H

#include "cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class hand_recognition {
    public:
    hand_recognition();
        ~hand_recognition(){};



    public:
        void extract_contour(Mat hand_roi);
        void get_convexHull(Mat img);
         vector<Point> hand_contours;
        void condense_frame(Mat frame);


    private:
        int flag_winD;

    public:
        bool  debug;

};


#endif //HAND_DETECT_RECOGNITION_HAND_EXTRACT_H
