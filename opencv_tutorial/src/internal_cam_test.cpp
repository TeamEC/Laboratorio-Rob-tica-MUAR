#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class cam_test{
  public:
    cam_test(){

    VideoCapture cap(CV_CAP_ANY); //abrir cámara
      if (!cap.isOpened())
      {
        cout << "No se puede abrir la cámara" << endl;

      }

    double dWith = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "Frame size: " << dWith << "x" << dHeight << endl;
    namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

      while (1)
      {
        Mat frame;
        bool bSuccess = cap.read(frame);

        if (!bSuccess)
        {
          cout << "Cannot read a frame from video stream" << endl;
          break;
        }

              imshow("MyVideo", frame);
        if (waitKey(30) == 27)
        {
          cout << "ESC key is pressed by user" << endl;
          break;
        }
      }
    }
    ~cam_test()
    {
        cvDestroyWindow("Camera Output");
    }
};

int main(int argc, char  **argv)
{
  ros::init(argc, argv,"internal_cam_test");
  cam_test cam_object;

  ROS_INFO("Cam Tested!");
}
