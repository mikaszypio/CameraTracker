#include "pch.h"
#include "FrmProc.h"
#include <opencv2/imgproc/imgproc.hpp>

FrmProc::FrmProc()
{
	isRunning = false;
	analyzer = FrmAnalyzer();
	scale = 1.05;
	cImage = new CImage();
	bitmapSizeSet = false;
	pictureBox = NULL;
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
		frame = ResizeFrm(capturedFrm, 640, 480);
		if (detectionStatus)
			frame = analyzer.DetectSilhouettes(frame, scale, weight, hitThresh, winStride, padding);
		if (pictureBox != NULL && bitmapSizeSet) DisplayImage();
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

void FrmProc::ReleaseImage(CImage* cimg)
{
	if (cimg)
	{
		cimg->ReleaseDC();
		delete cimg;
		cimg = NULL;
	}
}

void FrmProc::DisplayImage()
{
	HBITMAP bitImage = (HBITMAP)CreateHBitmap(frame);
	cImage->Attach(bitImage);
	cImage->BitBlt(GetDC(pictureBox->m_hWnd), 0, 0);
	cImage->Detach();
}

BITMAPINFO FrmProc::CreateBitmapInfo(cv::Mat frm)
{
	BITMAPINFOHEADER header = CreateBitmapHeader(frm);
	return CreateBitmapInfo(header);
}

BITMAPINFO FrmProc::CreateBitmapInfo(BITMAPINFOHEADER header)
{
	BITMAPINFO info = BITMAPINFO{};
	ZeroMemory(&info, sizeof(info));
	info.bmiHeader = header;
	info.bmiColors->rgbBlue = 0;
	info.bmiColors->rgbGreen = 0;
	info.bmiColors->rgbRed = 0;
	info.bmiColors->rgbReserved = 0;
	return info;
}


BITMAPINFOHEADER FrmProc::CreateBitmapHeader(cv::Mat frm)
{
	cv::Size frmSize = frm.size();
	BITMAPINFOHEADER header = BITMAPINFOHEADER{};
	ZeroMemory(&header, sizeof(header));
	header.biSize = sizeof(header);
	header.biWidth = frmSize.width;
	header.biHeight = -(frmSize.height);
	header.biPlanes = 1;
	header.biBitCount = frm.channels() * 8;
	return header;
}

unsigned int FrmProc::GetCvDepthVal(int depth)
{
	unsigned int value;
	switch (depth)
	{
	case CV_8U:
	case CV_8S: value = 8u; break;
	case CV_16U:
	case CV_16S: value = 16u; break;
	case CV_32S:
	case CV_32F: value = 32u; break;
	case CV_64F: value = 64u; break;
	default: break;
	}
	return value;
}

HBITMAP FrmProc::CreateHBitmap(cv::Mat frm)
{
	BITMAPINFOHEADER bitmapHeader = (BITMAPINFOHEADER)CreateBitmapHeader(frm);
	BITMAPINFO bitmapInfo = (BITMAPINFO)CreateBitmapInfo(bitmapHeader);
	HDC hDc = GetDC(nullptr);
	HBITMAP hBitmap = CreateDIBitmap(hDc, &bitmapHeader, CBM_INIT, frm.data, &bitmapInfo, DIB_RGB_COLORS);
	DeleteDC(hDc);
	return hBitmap;
}

void FrmProc::SetBitmapSize(cv::Size size)
{
	bitmapSize = size;
	bitmapSizeSet = true;
}

void FrmProc::SetPictureBox(CStatic* cstatic)
{
	pictureBox = cstatic;
}
