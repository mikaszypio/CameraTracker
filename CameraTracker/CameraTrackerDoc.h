
// CameraTrackerDoc.h: interfejs klasy CCameraTrackerDoc
//


#pragma once
#include "FrmProc.h"

class CCameraTrackerDoc : public CDocument
{
protected: // utwórz tylko na podstawie serializacji
	CCameraTrackerDoc() noexcept;
	DECLARE_DYNCREATE(CCameraTrackerDoc)
	void StartFrmProc(LPCTSTR lpszPathName);
	void StopFrmProc();
	double scale;
	double lThresh;
	double uThresh;
	cv::Size imgSize;
	cv::Mat m_cvImage;
	FrmProc* frmProc = nullptr;
// Atrybuty
public:
	void SetAnalyzerAttr(double scale, double weight, double hitThresh, int winStride, int padding);
	void SetProcPictureCtrl(CStatic* cstatic);
// Operacje
public:
	void SetImgSize(LONG width, LONG height);
	void SetAnalyzerOnOff(bool status);
	void SetPause(bool status);
	void SetAnalyzerGrouping(bool status);
// Przesłania
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

// Implementacja
public:
	virtual ~CCameraTrackerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Wygenerowano funkcje mapy komunikatów
protected:
	DECLARE_MESSAGE_MAP()
};
