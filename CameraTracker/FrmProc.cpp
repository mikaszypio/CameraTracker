#include "pch.h"
#include "FrmProc.h"
#include <opencv2/imgproc/imgproc.hpp>

FrmProc::FrmProc()
{
	isRunning = false;
	analyzer = FrmAnalyzer();
	scale = 1.05;
}

FrmProc* FrmProc::s_instance = NULL;
FrmProc* FrmProc::GetInstance()
{
	if (s_instance == NULL)
		s_instance = new FrmProc();

	return s_instance;
}

void FrmProc::RunThread(FrmProc* frmProc)
{
	frmProc->isRunning = true;
	frmProc->Run();
}

void FrmProc::StopThread(FrmProc* frmProc)
{
	if (frmProc->isRunning)
	{
		frmProc->isRunning = false;
		frmProc->thread.join();
	}
}

void FrmProc::Run()
{
	cv::Mat capturedFrm;
	SetVideoCapture();
	while (isRunning && videoCapture.read(capturedFrm))
	{
		cv::Mat frm = ResizeFrm(capturedFrm, 640, 480);
		frm = analyzer.DetectSilhouettes(frm, scale, weight, hitThresh, winStride, padding);
		cv::resize(frm, frm, cv::Size(640, 480), 0, 0, cv::INTER_LINEAR);
		cv::imshow("IDC_STATIC_OUTPUT", frm);
		cv::waitKey(15);
	}
	videoCapture.release();
}

void FrmProc::SetPathName(LPCTSTR lpszPathName)
{
	if (lpszPathName == nullptr)
		pathName.clear();
	else pathName = std::string(CT2A(lpszPathName));
}

cv::Mat FrmProc::ResizeFrm(cv::Mat frm, int minWidth, int minHeight)
{
	cv::Mat resizedFrm;
	cv::resize(frm, resizedFrm, cv::Size(minWidth, minHeight), 0, 0, cv::INTER_LINEAR);
	return resizedFrm;
}

void FrmProc::SetVideoCapture()
{
	if (videoCapture.isOpened())
		videoCapture.release();
	if (pathName.empty())
		videoCapture.open(0);
	else videoCapture.open(pathName);
}

void FrmProc::SetAttributes(double scale, double weight, double hitThresh, int winStride, int padding)
{
	this->scale = scale;
	this->weight = weight;
	this->hitThresh = hitThresh;
	this->winStride = cv::Size(winStride, winStride);
	this->padding = cv::Size(padding, padding);
}