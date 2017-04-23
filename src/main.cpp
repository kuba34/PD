//This is release version. For more info check parent of this commit
//The point of this version is to be deployed on device, so there is absolutely no debug or on-going set for params
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "cansend.h"

using namespace cv;
using namespace std;

VideoCapture camera_start();

int main() {
  VideoCapture capture=camera_start();
  u_int dist=0, flag=0;
  int treshl=20, treshh=60;
  int i_cont;
  double a, b, c;
	int cap_width, dir;
	char speed_addr[] = "A45";
	char dir_addr[] = "B45";

  std::vector<vector<Point> > contours;
  std::vector<Point> contours_poly;
  std::vector<Vec3f> circles;
  std::vector<KeyPoint> keypoints;
  Mat frame, binary, cont, hough, frame_c, detectable, frame_a, tmp;
  Rect boundRect;
  cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();
  
	while ( true ) {
    contours.clear();
    contours_poly.clear();
    circles.clear();

    capture >> frame_c;
		cap_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    cvtColor(frame_c, frame, CV_BGR2GRAY);
    blur(frame, frame_a, Size(3, 3));
    Canny(frame_a, binary, treshl, treshh, 3);
    binary.copyTo(cont);
    findContours( cont, contours, CV_CHAIN_APPROX_TC89_L1, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    while(contours.size()>0 ) {
      if ( abs(contourArea(Mat(contours[0]))) < 200 ) {
        contours.erase (contours.begin());
        continue;
      }
      i_cont = 0;
      approxPolyDP( Mat(contours[i_cont]), contours_poly, 3, true );
      boundRect = boundingRect( Mat(contours_poly) );
      if(((double)boundRect.width/(double)boundRect.height>=1.3)||((double)boundRect.width/(double)boundRect.height<=0.7)) {
        contours.erase(contours.begin());
        continue;
      }
      hough= Mat::zeros(cont.size(), CV_8UC1);
      hough=binary(Rect(boundRect.x, boundRect.y, boundRect.width, boundRect.height));
      HoughCircles(hough, circles, CV_HOUGH_GRADIENT, 2, binary.rows/4, 200, 100 );
      if (circles.size()==0) {
        contours.erase (contours.begin());
        continue;
      }
      while( circles.size()>0) {
        Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
        center.x+=boundRect.x;
        center.y+=boundRect.y;
        detectable=Mat::zeros(frame.size(), CV_8UC1);
        detectable=frame(Rect(boundRect.x, boundRect.y, boundRect.width, boundRect.height));
        detectable.copyTo(tmp);
        detector->detect( tmp, keypoints );
        if(keypoints.size()==3){
          a = pow((keypoints[0].pt.x-keypoints[1].pt.x), 2)+ pow((keypoints[0].pt.y-keypoints[1].pt.y), 2);
          b = pow((keypoints[1].pt.x-keypoints[2].pt.x), 2)+ pow((keypoints[1].pt.y-keypoints[2].pt.y), 2);
          c = pow((keypoints[2].pt.x-keypoints[0].pt.x), 2)+ pow((keypoints[2].pt.y-keypoints[0].pt.y), 2);
          if(abs(a-b)<25&&abs(a-c)<25&&abs(b-c)<25&&abs(b-a)<25&&abs(c-a)<25&&abs(c-b)<25) {
            cout<<endl<<"Found!!!: ";
            cout<<center<<endl;
            double mean= (a+b+c)/3;
            dist=10.5*609.262/mean;
            cout<<"Distance: "<<dist<<endl;
						dir = cap_width/2 - center.x;
						cansend(dist,speed_addr);
						cansend(dir,dir_addr);
            flag=1;
            break;
          }
          else {
            circles.erase (circles.begin());
            continue;
          }
        }
        else {
          circles.erase (circles.begin());
          continue;
        }
      }
      if(flag==1) {
        flag=0;
        break;
      }
      contours.erase (contours.begin());
    }
    if (contours.size()==0) {
    cout<<"Nie znalazłem ani jednego wzroca!"<<endl;
    }
  }
  capture.release();
  contours.clear();
  contours_poly.clear();
  circles.clear();
  return 0;
}

VideoCapture camera_start()
{
  VideoCapture capture = VideoCapture(0);
  if(!capture.isOpened()) {
    cerr<<"Kamera umarla"<<endl;
    abort();
  }
  else{
    return capture;
  }
}
