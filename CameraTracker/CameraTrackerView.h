
// CameraTrackerView.h: interfejs klasy CCameraTrackerView
//

#pragma once


class CCameraTrackerView : public CFormView
{
protected: // utwórz tylko na podstawie serializacji
	CCameraTrackerView() noexcept;
	DECLARE_DYNCREATE(CCameraTrackerView)
	CString GetLabelStr(UINT ids, int value = -1);
	void UpdateValues();
	bool SetCSlider(CSliderCtrl& cslider, CEdit& cedit);
	bool SetCEdit(CEdit& cedit, CSliderCtrl& cslider);
	bool ValidateCEditInput(CEdit& cedit);
	bool ValidateCEditInput(CString cstr);
public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_CAMERATRACKER_FORM };
#endif

// Atrybuty
public:
	CCameraTrackerDoc* GetDocument() const;

// Operacje
public:

// Przesłania
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	virtual void OnInitialUpdate(); // wywołany pierwszy raz po skonstruowaniu
	virtual void OnDraw(CDC* pDC);

// Implementacja
public:
	virtual ~CCameraTrackerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Wygenerowano funkcje mapy komunikatów
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sliderCtrlScale;
	CSliderCtrl m_sliderCtrlWeight;
	CSliderCtrl m_sliderCtrlHThresh;
	CStatic m_staticScaleVal;
	CStatic m_staticWeightVal;
	CStatic m_staticHitThreshVal;
	CSliderCtrl m_sliderCtrlWinStride;
	CSliderCtrl m_sliderCtrlPadding;
	CStatic m_staticWinStrideVal;
	CStatic m_staticPadding;
	afx_msg void OnEnKillfocusEdit();
	CEdit m_editCtrlScale;
	CEdit m_editCtrlWeight;
	CEdit m_editCtrlHThresh;
	CButton m_checkOffOn;
	CMFCButton m_MfcButtonPauseOn;
	CMFCButton m_MfcButtonPauseOff;
	afx_msg void OnEnChangeEditHthresh();
	afx_msg void OnEnUpdateEditHthresh();
};

#ifndef _DEBUG  // debuguj wersję w elemencie CameraTrackerView.cpp
inline CCameraTrackerDoc* CCameraTrackerView::GetDocument() const
   { return reinterpret_cast<CCameraTrackerDoc*>(m_pDocument); }
#endif

