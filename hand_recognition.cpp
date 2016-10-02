//
// Created by zzz on 2016/9/20.
//
#include <Windows.h>
#include "hand_recognition.h"
void hand_recognition::extract_contour(Mat hand_roi) {
//    Mat dst;
//    GaussianBlur( hand_roi, dst, Size( 3, 3 ), 0, 0 );
//    Canny(hand_roi,dst,60,60);
//    vector<vector<Point>> extract_edge;
//    findContours(dst,extract_edge,CV_RETR_EXTERNAL ,CV_CHAIN_APPROX_NONE);
//    this->contours=extract_edge;
//    if(debug)
//    {
//        Mat debug_mat(dst.rows,dst.cols,CV_8UC1,Scalar(0));
//        drawContours(debug_mat,extract_edge,-1,Scalar(255,100,100),1,8);
//        condense_frame(debug_mat);
//        imshow("debug_contours",debug_mat);
//    }

}
/*输入保证是8位单通道的手势图片*/
void hand_recognition::condense_frame(Mat frame) {

    if(frame.empty())
    {
        printf("frame is empty\n");
        return ;
    }

    int height=frame.rows;
    int weight=frame.cols;
    Mat condense_mat(frame.rows,frame.cols,CV_8UC1,Scalar(0));
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<weight;j++)
        {
            uchar value=*frame.ptr(i,j);
            if((i+1)%5==0&&(j+1)%5==0)//如果该坐标满足某个条件，就保留。
            {
//                printf("i+1=%d j+1=%d\n",i+1,j+1);
                if(value==255)
                {
//                    printf("value=%d\n",value);
//                    printf("working");
                    *condense_mat.ptr(i,j)=255;
                }
            }
        }
    }

    if(debug)
    {
        imshow("condense_mat",condense_mat);
        imshow("frame_before_condencse",frame);
    }

}
hand_recognition::hand_recognition() {
    debug=true;
    flag_winD=0;
}
void hand_recognition::get_convexHull(Mat img) {
    vector<int> hull;
    vector<Vec4i> defects;
    if(hand_contours.size()<=0)
        return;
    convexHull(Mat(hand_contours), hull, false);
    int sizehand_contours=hand_contours.size();
    int szie_hull=hull.size();

    convexityDefects(Mat(hand_contours),hull,defects);
    cvtColor(img,img,CV_GRAY2BGR);
    int number=0;
    for(int i=0;i<defects.size();i++)
    {
        if(defects[i][3]>15000) {
            circle(img, hand_contours[defects[i][2]], 5, Scalar(200, 200, 255), 2);
            circle(img, hand_contours[defects[i][1]], 5, Scalar(100, 100, 100), 2);
            number++;
        }
    }
    if(number>=3&&flag_winD==0) {
//            keybd_event(VK_LWIN, 0, 0, 0);
//            keybd_event('D', 0, 0, 0);
//            keybd_event('D', 0, KEYEVENTF_KEYUP, 0);
//            keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
//            flag_winD=1;
    }
    else if(number<=2&&flag_winD==1)
    {
//        keybd_event(VK_LWIN, 0, 0, 0);
//        keybd_event('D', 0, 0, 0);
//        keybd_event('D', 0, KEYEVENTF_KEYUP, 0);
//        keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
//        flag_winD=0;
    }
    if(debug)
    {
        imshow("hull",img);
    }

}