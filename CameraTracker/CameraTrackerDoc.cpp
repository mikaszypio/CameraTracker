
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

void CCameraTrackerDoc::SetNeighbors(int value)
{
	lThresh = value;
	frmProc->SetAttributes(scale, lThresh, uThresh);
}

void CCameraTrackerDoc::SetMinSize(int value)
{
	uThresh = value;
	frmProc->SetAttributes(scale, lThresh, uThresh);
}

void CCameraTrackerDoc::SetScaleFact(int value)
{
	scale = (double)value / 1000000;
	frmProc->SetAttributes(scale, lThresh, uThresh);
}

void CCameraTrackerDoc::SetAnalyzerAttr(double scale, double weight, double hitThresh, int winStride, int padding)
{
	/*this->scale = (double)scale / 1000000;
	this->uThresh = (double)uThreshold / 100;
	this->lThresh = (double)lThreshold / 100;
	frmProc->SetAttributes(this->scale, this->uThresh, this->lThresh);*/
}