#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

#pragma comment(lib, "opencv_core455.lib")
#pragma comment(lib, "opencv_videoio455.lib")
#pragma comment(lib, "opencv_highgui455.lib")
#pragma comment(lib, "opencv_imgproc455.lib")

using namespace cv;
using namespace std;

constexpr double cameraMaxWidth = 1280;
constexpr double cameraMaxHeight = 720;

#define CUSTOMIED_VIDEO_WIDTH 1600
#define CUSTOMIED_VIDEO_height 900

int main(int, char **)
{
	Mat frame;
	VideoCapture cap;
	VideoCapture &capture = cap;

	cap.open(0);
	cap.set(CAP_PROP_FRAME_WIDTH, cameraMaxWidth);
	cap.set(CAP_PROP_FRAME_HEIGHT, cameraMaxHeight);
	if (!cap.isOpened())
	{
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
	cout << "Start grabbing" << endl
		 << "Press any key to terminate" << endl;
	Mat newFrame;
	for (;;)
	{
		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		cv::resize(frame, newFrame, cv::Size(cameraMaxWidth / 3, cameraMaxHeight / 3));
		imshow("Live", newFrame);
		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}