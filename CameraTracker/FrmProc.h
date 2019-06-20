#pragma once
#include "FrmAnalyzer.h"
#include <opencv2/highgui/highgui.hpp>
#include <thread>

class FrmProc
{
protected:
	static FrmProc* s_instance;
	cv::VideoCapture videoCapture;
	FrmAnalyzer analyzer;
	bool isRunning;
	std::string pathName;
	FrmProc();
	void Run();
	void SetVideoCapture();
public:
	double scale;
	double weight;
	double hitThresh;
	cv::Size winStride;
	cv::Size padding;
	std::thread thread;
	static void RunThread(FrmProc* frmProc);
	static void StopThread(FrmProc* frmProc);
	static FrmProc* GetInstance();
	cv::Mat ResizeFrm(cv::Mat frm, int minWidth, int minHeight);
	void SetPathName(LPCTSTR lpszPathName);
	void SetAttributes(double scale, double weight, double hitThresh, int winStride, int padding);
};
