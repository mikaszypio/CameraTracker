
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
	FrmProc* frmProc = nullptr;
	double scale;
	double lThresh;
	double uThresh;
// Atrybuty
public:
	void SetScaleFact(int val);
	void SetNeighbors(int val);
	void SetMinSize(int val);
	void SetAnalyzerAttr(double scale, double weight, double hitThresh, int winStride, int padding);
// Operacje
public:
	
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
