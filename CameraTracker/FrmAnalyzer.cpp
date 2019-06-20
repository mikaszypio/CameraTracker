#include "pch.h"
#include "FrmAnalyzer.h"
#include <opencv2/imgproc/imgproc.hpp>

cv::Scalar FrmAnalyzer::colorBlue = cv::Scalar(255, 0, 0);
cv::Scalar FrmAnalyzer::colorRed = cv::Scalar(0, 0, 192);
FrmAnalyzer::FrmAnalyzer()
{
	hogDescriptor.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
	lastSilhouettes = std::vector<Pedestrian>();
}

Pedestrian FrmAnalyzer::DrawVector(cv::Mat frm, Pedestrian prevPed, Pedestrian ped)
{
	cv::Point p1 = prevPed.feature;
	cv::Point p2 = ped.feature;
	if (!ped.directions.empty())
	{
		p1 = ped.GetVector(p2);
		cv::circle(frm, p2, 10, colorRed, 3);
		cv::line(frm, p1, p2, colorRed, 3);
		ped.AddToDirections(p2);
	}
	return ped;
}

cv::Mat FrmAnalyzer::DetectSilhouettes(cv::Mat frm, double scale, double lThresh, double uThresh)
{
	std::vector<cv::Rect> bodies;
	std::vector<double> weights;
	std::vector<Pedestrian> pedestrians = std::vector<Pedestrian>();
	if (!lastSilhouettes.empty()) pedestrians = TrackSilhouettes(frm);
	hogDescriptor.detectMultiScale(frm, bodies, weights, 0.0, cv::Size(), cv::Size(), 1.05, 2.0);
	pedestrians = UpdateSilhouettes(bodies, pedestrians);
	std::vector<Pedestrian> silhouettes = MarkSilhouettes(frm, pedestrians);
	lastSilhouettes = pedestrians;
	features.clear();
	for (int i = 0; i < pedestrians.size(); i++)
		features.push_back(pedestrians[i].feature);

	return frm;
}

cv::Point FrmAnalyzer::GetCentralPoint(cv::Rect rect)
{
	int x = rect.x + ((rect.width) / 2);
	int y = rect.y + ((rect.height) / 2);
	return cv::Point(x, y);
}

cv::Point FrmAnalyzer::GetVectorEnd(cv::Point p1, cv::Point p2)
{
	int x = (p1.x - p2.x) * 25;
	int y = (p1.y - p2.y) * 25;
	return cv::Point(p2.x - x, p2.y - y);
}

std::vector<Pedestrian> FrmAnalyzer::MarkSilhouettes(cv::Mat frm, std::vector<Pedestrian> silhouettes)
{
	for (int i = 0; i < silhouettes.size(); i++)
	{
		cv::Point topLeft = silhouettes[i].GetTopLeft();
		cv::Point bottomRight = silhouettes[i].GetBottomRight();
		cv::rectangle(frm, topLeft, bottomRight, colorBlue, 3);
		if (!lastSilhouettes.empty())
		{
			for (int j = (int)lastSilhouettes.size() - 1; j > -1; j--)
			{
				float x0 = lastSilhouettes[j].feature.x;
				float y0 = lastSilhouettes[j].feature.y;
				float x = abs(silhouettes[i].feature.x - x0);
				float y = abs(silhouettes[i].feature.y - y0);
				if (x < 10 && y < 10)
				{
					Pedestrian ped = DrawVector(frm, lastSilhouettes[j], silhouettes[i]);
					lastSilhouettes.erase(lastSilhouettes.begin() + j);
					break;
				}
			}
		}
	}
	return silhouettes;
}

std::vector<Pedestrian> FrmAnalyzer::UpdateSilhouettes(std::vector<cv::Rect> bodies, std::vector<Pedestrian> pedestrians)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		cv::Point point = GetCentralPoint(bodies[i]);
		Pedestrian ped = Pedestrian(bodies[i]);
		ped.feature = point;
		int index = IsDuplicate(ped, pedestrians);
		if (index != -1)
		{
			pedestrians[index].feature = point;
			pedestrians[index].SetRect(bodies[i]);
		}
		else
		{
			Pedestrian pedestrian = Pedestrian(bodies[i]);
			pedestrian.AddToDirections(point);
			pedestrian.feature = point;
			pedestrians.push_back(pedestrian);
		}
	}
	return pedestrians;
}

std::vector<Pedestrian> FrmAnalyzer::TrackSilhouettes(cv::Mat frm)
{
	#pragma region Variables_init
	std::vector<Pedestrian> newPedestrians = std::vector<Pedestrian>();
	cv::Mat gray;
	std::vector<cv::Point2f> newFeatures;
	std::vector<uchar> status;
	std::vector<float> err;
	#pragma endregion
	cv::cvtColor(frm, gray, cv::COLOR_BGR2GRAY);
	if (!oldGray.empty())
	{
		cv::calcOpticalFlowPyrLK(oldGray, gray, features, newFeatures, status, err);
		for (int i = 0; i < status.size(); i++)
		{
			//int index = IsDuplicate(newFeatures[i], lastSilhouettes);
			int index = FindPedestrian(newFeatures[i], lastSilhouettes);
			if (index != -1 && !lastSilhouettes[index].Equals(newFeatures[i]))
			{
				lastSilhouettes[index].feature = newFeatures[i];
				newPedestrians.push_back(lastSilhouettes[index]);
			}
		}
	}
	oldGray = gray.clone();
	return newPedestrians;
}

int FrmAnalyzer::FindPedestrian(cv::Point2f feature, std::vector<Pedestrian> pedestrians)
{
	int index;
	for (int i = (int)pedestrians.size()-1; i > -1; i--)
	{
		if (pedestrians[i].InRange(feature, 10))
		{
			return i;
		}
	}

	return -1;
}

int FrmAnalyzer::IsDuplicate(Pedestrian ped, std::vector<Pedestrian> pedestrians)
{
	for (int i = 0; i < pedestrians.size(); i++)
	{
		if (pedestrians[i].InRange(ped.feature) || ped.InRange(pedestrians[i].feature))
		{
			return i;
		}
	}
	return -1;
}