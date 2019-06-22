#pragma once
#include <opencv2/opencv.hpp>

class Pedestrian
{
protected:
	cv::Rect rect;
	cv::Point AveragePoint();
public:
	std::vector<cv::Point> directions;
	cv::Point2f feature;
	Pedestrian(cv::Rect rect);
	Pedestrian(cv::Rect rect, std::vector<cv::Point> directions);
	cv::Rect GetRect();
	void SetRect(cv::Rect rect);
	cv::Point GetVector(cv::Point p0);
	void AddToDirections(cv::Point p);
	bool InRange(cv::Point2f feature, int limit);
	bool InRange(cv::Point2f feature);
	cv::Point GetTopLeft();
	cv::Point GetBottomRight();
	bool Equals(cv::Point p);
	bool GotBiggerRect(cv::Rect rect);
	bool verified;
	cv::Scalar color;
};

