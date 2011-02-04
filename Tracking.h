#ifndef TRACKING_H_
#define TRACKING_H_

#include <iostream>
#include <list>

#include "cv.h"
#include "highgui.h"

#include "Marker.h"

#include "PoseEstimation.h"

class Tracking
{
public:
	Tracking();
	~Tracking(){};

	cv::Mat getFrame();

	/**
	 * Saves all markers that are found in the current frame into the given vector
	 */
	void getMarkers(std::list<Marker> &m);

private:
	cv::VideoCapture* video;

	static float sobelKernelArray[3][3];
	cv::Mat sobelKernel;	//sobel filtering kernel

	cv::Mat frame;

	void toCstyle(cv::Point2f* in, CvPoint2D32f* out, int length);
	unsigned short getMarkerID(cv::Mat &marker, int* rotations);
	void getCorners(cv::Vec4f* borders, cv::Point2f* corners);
	cv::Vec4f getAccurateBorder(cv::Point start, cv::Point end, cv::Mat &input, cv::Mat &output);
	cv::Point2f getSubpixelBorderPosFromStripe(cv::Mat &input, cv::Point2f divCenter, cv::Point2f stripeX, cv::Point2f stripeY, int stripeWidth, int stripeHeight);
	cv::Point2f getEdgePos(const cv::Mat& stripe);
	int subpixSampleSafe ( const IplImage* pSrc, cv::Point2f p );
	float length(cv::Point p);
};

#endif
