//
// Created by zzz on 2016/9/20.
//
#include "hand_detect.h"
bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;
Mat img;
Mat image;


static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);
        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
        case CV_EVENT_LBUTTONDOWN:
            origin = Point(x,y);
            selection = Rect(x,y,0,0);
            selectObject = true;
            break;
        case CV_EVENT_LBUTTONUP:
            selectObject = false;
            if( selection.width > 0 && selection.height > 0 )
                trackObject = -1;
            break;
    }
}
VideoCapture cap(0);
Rect trackWindow;
int hsize = 16;
float hranges[] = {0,180};
const float* phranges = hranges;
Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
bool paused = false;
hand_detect::hand_detect() {
    namedWindow( "Histogram", 0 );
    namedWindow( "CamShift Demo", 0 );
    namedWindow( "fff", 0 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );
    he=new hand_recognition();
}
hand_detect::~hand_detect() {
    delete(he);
    he=NULL;
    delete(img);
    img=NULL;

}
void hand_detect::pre_deal() {
    int _vmin = vmin, _vmax = vmax;
    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),    //得到选择的块
            Scalar(180, 256, MAX(_vmin, _vmax)), mask);//如果图像的像素点在范围内，则掩模版返回1.
    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    mixChannels(&hsv, 1, &hue, 1, ch, 1);//将通道0从hsv拷贝到hue0通道
}
void hand_detect::get_frame(Mat frame) {

    if(frame.empty())
    {
        printf("img is empty... exit..");
        flag=false;
        return;
    }

}

void hand_detect::get_roi() {

    Mat roi(hue, selection), maskroi(mask, selection);
    this->roi=roi;
    this->maskroi=maskroi;
}
void hand_detect::get_handroi(Mat &f) {
    Point leftup_pt(hand_region.x,hand_region.y);
    if(leftup_pt.x+hand_region.width>image.cols)
        hand_region.width=image.cols-leftup_pt.x;
    if(leftup_pt.y+hand_region.height>image.rows)
        hand_region.height=image.rows-leftup_pt.y;
}
void hand_detect::get_histogram() {
        calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);//第一个参数为源图像，第二个参数源图像个数，第三个参数取每个图像的0通道统计
        normalize(hist, hist, 0, 255, CV_MINMAX);
        int binW = histimg.cols / hsize;//计算宽
        Mat buf(1, hsize, CV_8UC3);//向量？
        histimg = Scalar::all(0);
        for( int i = 0; i < hsize; i++ )//saturate_cast作用防止溢出，如果结果为负，转0，超255，转255
            buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);//给每一维的每一分度划分颜色界限
        cvtColor(buf, buf, CV_HSV2BGR);
        //以下是画直方图
        for( int i = 0; i < hsize; i++ )
        {
            int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
            rectangle( histimg, Point(i*binW,histimg.rows),
                       Point((i+1)*binW,histimg.rows - val),
                       Scalar(buf.at<Vec3b>(i)), -1, 8 );
        }
         imshow("roi",roi);
}
void hand_detect::get_init_pt() {
        this->init_pt=selection;
        trackObject = 1;
}
Mat background;
void hand_detect::back_ground(Mat frame) {
    Mat foreground;
    mog(frame,foreground,0.000015);
    erode(foreground, foreground, cv::Mat());
    dilate(foreground, foreground, cv::Mat());
    mog.getBackgroundImage(background);
    if(he->debug)
    {
        imshow("foreground",foreground);
        imshow("background",background);
    }
}
void hand_detect::track_hand() {

    calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
    imshow("backpro",backproj);
    RotatedRect trackBox = CamShift(backproj, init_pt,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
    circle(image,trackBox.center,8,Scalar(0,50,50));

    if( trackWindow.area() <= 1 )
    {
        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
        trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                           trackWindow.x + r, trackWindow.y + r) &
                      Rect(0, 0, cols, rows);
    }
    if( backprojMode )
        cvtColor( backproj, image, COLOR_GRAY2BGR );
    ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
    Rect min=trackBox.boundingRect();//得到左上角的坐标有可能是负数
    rectangle(image,min,Scalar(100,0,0));
    hand_region=min;
    if(hand_region.x<0)
        hand_region.x=0;
    if(hand_region.y<0)
        hand_region.y=0;
    Point left_up(hand_region.x,hand_region.y);
    if((left_up.x+hand_region.width)>=image.cols)
    {
        hand_region.width=image.cols-hand_region.x;
    }
    if((left_up.y+hand_region.height)>=image.rows)
    {
        hand_region.height=image.rows-hand_region.y;
    }
}

Mat  hand_detect::detect_by_color(Mat frame) {
    int h=frame.rows;
    int w=frame.cols;
    medianBlur(frame,frame,3);
    Mat img;
    cvtColor(frame,img,CV_RGB2HSV);
    Mat detect=Mat::zeros(Size(w,h),CV_8UC1);
    for(int i=0;i<h;i++)
    {
        for(int j=0;j<w;j++)
        {
            uchar value0=*img.ptr(i,j);
            if(value0<180&&value0>100)
                *detect.ptr(i,j)=255;
        }
    }
    Mat dst;
    Mat stru=getStructuringElement(CV_SHAPE_ELLIPSE,Size(5,5));
    morphologyEx(detect,dst,CV_MOP_OPEN,stru);
    if(he->debug)
    {
        imshow("detectfor_color",detect);
        imshow("reduce_minArea",dst);
    }
    return dst;
}
void hand_detect::reduce_minArea(Mat &frame) {
    int h=frame.rows;
    int w=frame.cols;
    vector<vector<Point>> contour;
    vector<vector<Point>>contours;
    Mat o=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
    findContours(frame,contour,CV_RETR_EXTERNAL ,CV_CHAIN_APPROX_NONE);
    drawContours(o,contour,-1,Scalar(255),5,8);
    imshow("0",o);
    int i=0;
    while(i<contour.size())
    {
        double area=contourArea(contour[i]);
        if(area>10000)
        {
            contours.push_back(contour[i]);
       //     printf("size=%d ",contours.size());
        }
                i++;
    }
    Mat out=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
    drawContours(out,contours,-1,Scalar(255,100,100),5,8);
    if(contours.size()==1)
    he->hand_contours=contours.at(0);
    if(he->debug)
    {
        imshow("draw_minArea",out);
    }
}
void hand_detect::meanshift_init() {
    VideoCapture cap(0);
    Mat frame;
    Mat img;
    if(!cap.isOpened())
    {
        printf("video no open");
    }
    int number=0;
    while(1) {
        cap>>frame;
        frame.copyTo(this->frame);
        if(frame.empty())
        {
            continue;
        }
        this->frame.copyTo(image);
        this->frame.copyTo(img);
        cvtColor(image, hsv, COLOR_BGR2HSV);
        back_ground(this->frame);
        imshow("CamShift Demo", image);
        imshow("Histogram", histimg);
        waitKey(25);
    }


}