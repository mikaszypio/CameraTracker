#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include "Pedestrian.h"

class FrmAnalyzer
{
private:
	cv::Mat oldGray;
	std::vector<cv::Point2f> features;
	int FindPedestrian(cv::Point2f feature, std::vector<Pedestrian> pedestrians);
	int IsDuplicate(Pedestrian ped, std::vector<Pedestrian> pedestrians);
protected:
	cv::HOGDescriptor hogDescriptor;
	static cv::Scalar colorBlue;
	static cv::Scalar colorRed;
	std::vector<Pedestrian> lastSilhouettes;
	Pedestrian DrawVector(cv::Mat frm, Pedestrian prevPed, Pedestrian ped);
	cv::Point GetCentralPoint(cv::Rect rect);
	cv::Point GetVectorEnd(cv::Point p1, cv::Point p2);
	std::vector<Pedestrian> MarkSilhouettes(cv::Mat frm, std::vector<Pedestrian> silhouettes);
	std::vector<Pedestrian> TrackSilhouettes(cv::Mat frm);
	std::vector<Pedestrian> UpdateSilhouettes(std::vector<cv::Rect> bodies, std::vector<Pedestrian> pedestrians);
public:
	FrmAnalyzer();
	cv::Mat DetectSilhouettes(cv::Mat frm, double scale = 1.05, double weight = -1, double hitThresh = 0,
		cv::Size winStride = cv::Size(), cv::Size padding = cv::Size(), bool grouping = false);
};

