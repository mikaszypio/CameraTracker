
// CameraTrackerDoc.cpp: implementacja klasy CCameraTrackerDoc
//

#include "pch.h"
#include "framework.h"
#include "CameraTracker.h"
#include "CameraTrackerDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCameraTrackerDoc

IMPLEMENT_DYNCREATE(CCameraTrackerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCameraTrackerDoc, CDocument)
END_MESSAGE_MAP()


// Tworzenie/niszczenie obiektu CCameraTrackerDoc

CCameraTrackerDoc::CCameraTrackerDoc() noexcept
{
	FrmProc* frmProc = FrmProc::GetInstance();
	lThresh = 3;
	scale = 1.05;
	uThresh = 0;
}

CCameraTrackerDoc::~CCameraTrackerDoc()
{
}

void CCameraTrackerDoc::StartFrmProc(LPCTSTR lpszPathName)
{
	StopFrmProc();
	frmProc->SetPathName(lpszPathName);
	frmProc->thread = std::thread(&FrmProc::RunThread, frmProc);
}

void CCameraTrackerDoc::StopFrmProc()
{
	frmProc->StopThread(frmProc);
}

BOOL CCameraTrackerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (frmProc == nullptr)
	{
		frmProc = FrmProc::GetInstance();
	}
	StartFrmProc(nullptr);
	return TRUE;
}

BOOL CCameraTrackerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (frmProc == nullptr)
	{
		frmProc = FrmProc::GetInstance();
	}
	StartFrmProc(lpszPathName);
	return TRUE;
}

// Serializacja klasy CCameraTrackerDoc

void CCameraTrackerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: W tym miejscu dodaj kod przechowywania
	}
	else
	{
		// TODO: W tym miejscu dodaj kod ładujący
	}
}

// Diagnostyka klasy CCameraTrackerDoc

#ifdef _DEBUG
void CCameraTrackerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCameraTrackerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// Polecenia CCameraTrackerDoc

void CCameraTrackerDoc::SetAnalyzerAttr(double scale, double weight, double hitThresh, int winStride, int padding)
{
	frmProc->SetAttributes(scale, weight, hitThresh, winStride, padding);
}

void CCameraTrackerDoc::SetImgSize(LONG width, LONG height)
{
	imgSize = cv::Size(width, height);
	frmProc->SetBitmapSize(imgSize);
}

void CCameraTrackerDoc::SetProcPictureCtrl(CStatic* cstatic)
{
	frmProc->SetPictureBox(cstatic);
}

void CCameraTrackerDoc::SetAnalyzerOnOff(bool status)
{
	frmProc->detectionStatus = status;
}