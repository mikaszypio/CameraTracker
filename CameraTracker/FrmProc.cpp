#include "pch.h"
#include "FrmProc.h"
#include <codecvt>
#include <locale>
#include <opencv2/imgproc/imgproc.hpp>

FrmProc::FrmProc()
{
	isRunning = false;
	analyzer = FrmAnalyzer();
	scale = 1.05;
	cImage = new CImage();
	bitmapSizeSet = false;
	pictureBox = nullptr;
}

FrmProc* FrmProc::s_instance = nullptr;
FrmProc* FrmProc::GetInstance()
{
	if (s_instance == nullptr)
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
		frmProc->clear();
	}
}

void FrmProc::clear()
{
	analyzer.clear();
}

void FrmProc::Run()
{
	cv::Mat capturedFrm;
	SetVideoCapture();
	while (isRunning)
	{
		try
		{
			if (!isPaused)
			{
				videoCapture >> capturedFrm;
				frame = ResizeFrm(capturedFrm, 640, 480);
				if (detectionStatus)
					frame = analyzer.DetectSilhouettes(frame, scale, weight, hitThresh, winStride, padding, grouping);
				if (pictureBox != nullptr && bitmapSizeSet) DisplayImage();
				cv::waitKey(15);
			}
		}
		catch (cv::Exception e)
		{

		}
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
	if (winStride != 0) this->winStride = cv::Size(winStride, winStride);
	else this->winStride = cv::Size();
	if (padding != 0) this->padding = cv::Size(padding, padding);
	else this->padding = cv::Size();
}

void FrmProc::ReleaseImage(CImage* cimg)
{
	if (cimg)
	{
		cimg->ReleaseDC();
		delete cimg;
		cimg = nullptr;
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
	unsigned int value = 0u;
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

void FrmProc::SetAsMarked(cv::Point point)
{
	analyzer.MarkSilhouette(point);
}

void FrmProc::StartRecording()
{
	std::string homepath = GetHomePath();
	std::string title = GetTitle();
	std::string extension = ".avi";
	std::stringstream stream;
	stream << homepath << title << extension;
	std::string pathname = stream.str();
	if (videoWriter.isOpened()) StopRecording();
	videoWriter.open(pathname, cv::VideoWriter::fourcc('M', 'P', 'E', 'G'), 20, bitmapSize, true);
}

void FrmProc::StopRecording()
{
	if (videoWriter.isOpened())
	{
		videoWriter.release();
	}
}

std::string FrmProc::GetTitle()
{
	std::stringstream stream;
	SYSTEMTIME time;
	GetLocalTime(&time);
	stream << "\\" << time.wDay << "-" << time.wMonth << "-" << time.wYear <<
		"_Time_=_" << time.wHour << "-" << time.wMinute << "-" << time.wSecond;
	return stream.str();
}

std::string FrmProc::GetHomePath()
{
	wchar_t* home;
	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &home);
	std::wstringstream wstream(home);
	CoTaskMemFree(static_cast<void*>(home));
	return CW2A(wstream.str().c_str());
}
