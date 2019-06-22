#include "pch.h"
#include "Pedestrian.h"

Pedestrian::Pedestrian(cv::Rect rect)
{
	directions = std::vector<cv::Point>();
	this->rect = rect;
	feature = cv::Point(-1, -1);
	verified = false;
}

Pedestrian::Pedestrian(cv::Rect rect, std::vector<cv::Point> directions)
{
	this->rect = rect;
	this->directions = directions;
	feature = directions.back();
	verified = false;
}

cv::Rect Pedestrian::GetRect()
{
	return rect;
}

void Pedestrian::SetRect(cv::Rect rect)
{
	this->rect = rect;
}

cv::Point Pedestrian::GetVector(cv::Point p0)
{
	cv::Point pavg = AveragePoint();
	int x = (pavg.x - p0.x) * 5;
	int y = (pavg.y - p0.y) * 5;
	return cv::Point(p0.x - x, p0.y - y);
}

void Pedestrian::AddToDirections(cv::Point p)
{
	directions.push_back(p);
	if (directions.size() > 5)
	{
		directions.erase(directions.begin());
	}
}

cv::Point Pedestrian::AveragePoint()
{
	int sumX = 0;
	int sumY = 0;
	for (int i = 0; i < directions.size(); i++)
	{
		sumX += directions[i].x;
		sumY += directions[i].y;
	}
	return cv::Point(sumX / (int)directions.size(), sumY / (int)directions.size());
}

bool Pedestrian::InRange(cv::Point2f feature, int limit)
{
	if (feature.y != -1 && feature.x != -1)
		if (abs(this->feature.x - feature.x) <= limit
			&& abs(this->feature.y - feature.y) <= limit)
			return true;
	return false;
}

bool Pedestrian::InRange(cv::Point2f feature)
{
	if (feature.y != -1 && feature.x != -1)
	{
		bool x = abs(this->feature.x - feature.x) <= this->rect.width / 2;
		bool y = abs(this->feature.y - feature.y) <= this->rect.height / 2;
		if (x && y) return true;
	}
	return false;
}

cv::Point Pedestrian::GetTopLeft()
{
	float x = feature.x - rect.width / 2;
	float y = feature.y - rect.height / 2 + 15;
	return cv::Point((int)x, (int)y);
}

cv::Point Pedestrian::GetBottomRight()
{
	float x = feature.x + rect.width / 2;
	float y = feature.y + rect.height / 2 + 15;
	return cv::Point((int)x, (int)y);
}

bool Pedestrian::Equals(cv::Point p)
{
	return feature.x == p.x && feature.y == p.y;
}

bool Pedestrian::GotBiggerRect(cv::Rect rect)
{
	bool width = rect.width < this->rect.width;
	bool height = rect.height < this->rect.width;
	return width || height;
}