#pragma once
#include "FrmAnalyzer.h"
#include <opencv2/highgui/highgui.hpp>
#include <thread>

class FrmProc
{
protected:
	static FrmProc* s_instance;
	CStatic* pictureBox;
	cv::VideoCapture videoCapture;
	FrmAnalyzer analyzer;
	bool isRunning;
	std::string pathName;
	cv::Size bitmapSize;
	CImage* cImage;
	cv::Mat frame;
	bool bitmapSizeSet;

	FrmProc();
	HBITMAP CreateHBitmap(cv::Mat frm);
	BITMAPINFO CreateBitmapInfo(cv::Mat frm);
	BITMAPINFO CreateBitmapInfo(BITMAPINFOHEADER header);
	BITMAPINFOHEADER CreateBitmapHeader(cv::Mat frm);
	unsigned int GetCvDepthVal(int depth);
	cv::Mat ResizeFrm(cv::Mat frm, int minWidth, int minHeight);
	void ReleaseImage(CImage* cimg);
	void Run();
	void SetVideoCapture();
public:
	double scale;
	double weight;
	double hitThresh;
	cv::Size winStride;
	cv::Size padding;
	std::thread thread;

	void DisplayImage();
	static FrmProc* GetInstance();
	static void RunThread(FrmProc* frmProc);
	static void StopThread(FrmProc* frmProc);
	void SetBitmapSize(cv::Size size);
	void SetPictureBox(CStatic* cstatic);
	void SetPathName(LPCTSTR lpszPathName);
	void SetAttributes(double scale, double weight, double hitThresh, int winStride, int padding);
};
