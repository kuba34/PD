//This is release version. For more info check parent of this commit
//The point of this version is to be deployed on device, so there is absolutely no debug or on-going set for params
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
//using namespace std::cout;
//using namespace std::cerr;
using namespace std;

VideoCapture camera_start();
float calc_speed_reg(float dist);
float calc_direction_reg(float width,float val);
char* can_msg(float reg, const char* can_device_id);

int main() {
  VideoCapture capture=camera_start();
  u_int dist=0, flag=0;
  char key='1';
  int treshl=20, treshh=60;
  int i_cont;
  double a, b, c;
	float cap_width, speed_reg, dir_reg;
	const char* speed_addr = "A45";
	const char* dir_addr = "B45";
	char msg[50];
	char can_s[100] = "cansend can0 ";

  std::vector<vector<Point> > contours;
  std::vector<Point> contours_poly;
  std::vector<Vec3f> circles;
  std::vector<KeyPoint> keypoints;
  Mat frame, binary, cont, hough, frame_c, detectable, frame_a, tmp;
  Rect boundRect;
  cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();
  while ( key!='q' ) {
    key=waitKey(30);
    contours.clear();
    contours_poly.clear();
    circles.clear();

    capture >> frame_c;
    cvtColor(frame_c, frame, CV_BGR2GRAY);
    blur(frame, frame_a, Size(3, 3));
    Canny(frame_a, binary, treshl, treshh, 3);
    binary.copyTo(cont);
    findContours( cont, contours, CV_CHAIN_APPROX_TC89_L1, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		cap_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    
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
          a = sqrt(pow((keypoints[0].pt.x-keypoints[1].pt.x), 2)+ pow((keypoints[0].pt.y-keypoints[1].pt.y), 2));
          b = sqrt(pow((keypoints[1].pt.x-keypoints[2].pt.x), 2)+ pow((keypoints[1].pt.y-keypoints[2].pt.y), 2));
          c = sqrt(pow((keypoints[2].pt.x-keypoints[0].pt.x), 2)+ pow((keypoints[2].pt.y-keypoints[0].pt.y), 2));
          if(abs(a-b)<5&&abs(a-c)<5&&abs(b-c)<5&&abs(b-a)<5&&abs(c-a)<5&&abs(c-b)<5) {
            cout<<endl<<"Found!!!: ";
            cout<<center<<endl;
            double mean= (a+b+c)/3;
            dist=10.5*609.262/mean;
            cout<<"Distance: "<<dist<<endl;
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
				float c = (float)center.x;
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
		speed_reg = calc_speed_reg(dist);
		dir_reg = calc_direction_reg(c, cap_width);
		can_msg(speed_reg, speed_addr);
  }
  capture.release();
  contours.clear();
  contours_poly.clear();
  circles.clear();
  return 0;
}

VideoCapture camera_start(){
  VideoCapture capture = VideoCapture(0);
  if(!capture.isOpened()) {
    cerr<<"Kamera umarla"<<endl;
    abort();
  }
  else{
    return capture;
  }
}

float calc_speed_reg(float dist)
{
	static const float Kd = 0.5;
	static const float Kp = 1;
	static float prev_error;
	static const float dist_point = 100;
	
	// PD controller
	float error = dist_point - dist;
  float d = (error - prev_error) * Kd;
	float p = Kp * error;

	prev_error = error;
	return d + p;
}

float calc_direction_reg(float val, float width)
{
	static const float Kd = 0.5;
	static const float Kp = 1;
	static float prev_error;
	static float center = width/2;
	
	// PD controller
	float error = center - val;
  float d = (error - prev_error) * Kd;
	float p = Kp * error;
	
	prev_error = error;
	return d + p;
}

char* can_msg(float reg, const char* can_device_id)
{
	char msg[50], reg_c[20];
	char s[100] = "cansend can0 ";
	sprintf(reg_c,"%d",reg);
	strcpy(msg,can_device_id);
	strcat(msg,"#");
	strcat(msg,reg_c);
	strcat(s,msg);
	
	system(s);
}
