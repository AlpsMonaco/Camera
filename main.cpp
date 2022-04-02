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

Mat addSaltNoise(const Mat src, int n);

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
	Mat &src = newFrame;

	for (;;)
	{
		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		cv::resize(frame, newFrame, Size(), 0.4, 0.4, INTER_LINEAR);

		// 增加椒盐噪声
		// Mat srcSaltPepper = addSaltNoise(src, 100);
		Mat &srcSaltPepper = src;
		// 中值滤波
		Mat dstMedian;
		medianBlur(srcSaltPepper, dstMedian, 3);
		Mat dstGaussian;
		GaussianBlur(srcSaltPepper, dstGaussian, Size(3, 3), 3, 3);
		// 双边滤波
		Mat dstBilateralFilter;
		bilateralFilter(srcSaltPepper, dstBilateralFilter, 25, 25 * 2, 25 / 2);

		namedWindow("src", WINDOW_AUTOSIZE);
		imshow("src", src);
		namedWindow("srcSaltPepper", WINDOW_AUTOSIZE);
		imshow("srcSaltPepper", srcSaltPepper);
		namedWindow("medianBlur", WINDOW_AUTOSIZE);
		imshow("medianBlur", dstMedian);
		namedWindow("GaussianBlur", WINDOW_AUTOSIZE);
		imshow("GaussianBlur", dstGaussian);
		namedWindow("bilateralFilter", WINDOW_AUTOSIZE);
		imshow("bilateralFilter", dstBilateralFilter);

		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}

Mat addSaltNoise(const Mat src, int n)
{

	Mat dst = src.clone();
	for (int k = 0; k < n; k++)
	{
		// 随机选择行列
		int i = rand() % dst.rows;
		int j = rand() % dst.cols;

		if (dst.channels() == 1)
		{
			dst.at<uchar>(i, j) = 255;
			// 盐噪声
		}
		else
		{
			dst.at<Vec3b>(i, j)[0] = 255;
			dst.at<Vec3b>(i, j)[1] = 255;
			dst.at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int k = 0; k < n; k++)
	{
		//随机取值行列
		int i = rand() % dst.rows;
		int j = rand() % dst.cols;
		//图像通道判定
		if (dst.channels() == 1)
		{
			dst.at<uchar>(i, j) = 0;
			// 椒噪声
		}
		else
		{
			dst.at<Vec3b>(i, j)[0] = 0;
			dst.at<Vec3b>(i, j)[1] = 0;
			dst.at<Vec3b>(i, j)[2] = 0;
		}
	}
	return dst;
}
