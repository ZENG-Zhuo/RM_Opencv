#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
const double err = 0.01;
const double WhiteL = 0.5;
const double BlackL = 0.2;
double max(double a, double b, double c)
{
    double temp = a;
    if (b>temp) temp = b;
    if (c>temp) temp = c;
    return temp;
} 
double min(double a, double b, double c)
{
    double temp = a;
    if (b<temp) temp = b;
    if (c<temp) temp = c;
    return temp;
} 
static void RGBtoHSL(int R, int G, int B, double &H, double &S, double &L)
{
    double r1 = R/255.0;
    double g1 = G/255.0;
    double b1 = B/255.0;
    double maxVal = max(r1,g1,b1);
    double minVal = min(r1,g1,b1);
    if(maxVal - minVal < err) H = 0;
    else if(maxVal - r1 < err && g1 >= b1) H = 60.0*(g1-b1)/(maxVal-minVal);
    else if(maxVal - r1 < err && g1 < b1) H = 60.0*(g1-b1)/(maxVal-minVal)+360.0;
    else if(maxVal - g1 < err) H = 60.0*(b1-r1)/(maxVal-minVal) + 120.0;
    else if(maxVal - b1 < err) H = 60.0*(r1-g1)/(maxVal-minVal) +240.0;
    L = (maxVal+minVal)/2.0;
    if(L - 0 <err || maxVal - minVal < err) S = 0;
    else if(L > 0 && L < 0.5) S = (maxVal - minVal)/(maxVal + minVal);
    else if (L > 0.5) S = (maxVal - minVal)/(2 - (maxVal + minVal));
}
int checkIfIsBorW(Vec3b pix) //W:2 B:0 Others:1
{
    double h,s,l;
    RGBtoHSL(pix[0]+0,pix[1]+0,pix[2]+0,h,s,l);
    if (l>WhiteL) return 2;
    else if (l<BlackL) return 0;
    else return 1;
}
int main()
{
    cv::VideoCapture cap;
    cap.open(0);
    //  if(!cap.isOpened())
    // {
    //     std::cout<<"video not open."<<std::endl;
    //     return 1;
    // }
    // cv::Mat image = cv::Mat::ones(cv::Size(100,100), CV_8UC3) * 255;
    // cv::Vec3b pixelColor = image.at<cv::Vec3b>(1,1);
    // std::cout << pixelColor[0]+0 << "\n";
    cv::Mat frame,result;
    // cap.read(frame);
    // cv::imshow("Display window", frame);
    // cv::waitKey(0);
    // double h,s,l;
    // RGBtoHSL(100,120,140,h,s,l);
    // std::cout<< h<<" "<<s<<" "<<l<<"\n";
    vector<Vec3b> rgb_pixels;
    rgb_pixels = {
        {0,0,0},
        {255,0,0},
        {255,255,255}
    };
    
    while (cap.isOpened())
    {
        cap.read(frame);
        result = Mat::eye(frame.rows,frame.cols,CV_8UC3);
        for (int i = 0; i< frame.rows; i++)
        {
            for (int j=0;j< frame.cols;j++)
            result.at<Vec3b>(i,j) = rgb_pixels[checkIfIsBorW(frame.at<Vec3b>(i,j))];
        }
        erode(result, result, Mat(), Point(-1,-1), 10);
        
        cv::imshow("Capture", result);
        if(cv::waitKey(1) == 'q')
            break;
    }
}