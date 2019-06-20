
// CameraTrackerView.cpp: implementacja klasy CCameraTrackerView
//

#include "pch.h"
#include "framework.h"
#include "CameraTracker.h"
#include "CameraTrackerDoc.h"
#include "CameraTrackerView.h"
#include <opencv2/highgui/highgui_c.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef CAMTRACK_DIVIDER
#define CAMTRACK_DIVIDER 1000000
#endif

// CCameraTrackerView

IMPLEMENT_DYNCREATE(CCameraTrackerView, CFormView)

BEGIN_MESSAGE_MAP(CCameraTrackerView, CFormView)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_WEIGHTS, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_HTHRESH, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_UPDATE(IDC_EDIT_HTHRESH, &CCameraTrackerView::OnEnUpdateEditHthresh)
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CCameraTrackerView

CCameraTrackerView::CCameraTrackerView() noexcept
	: CFormView(IDD_CAMERATRACKER_FORM)
{
}

CCameraTrackerView::~CCameraTrackerView()
{
}

void CCameraTrackerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_sliderCtrlScale);
	DDX_Control(pDX, IDC_SLIDER_WEIGHT, m_sliderCtrlWeight);
	DDX_Control(pDX, IDC_SLIDER_HITTHRESH, m_sliderCtrlHThresh);
	DDX_Control(pDX, IDC_STATIC_SCALE, m_staticScaleVal);
	DDX_Control(pDX, IDC_STATIC_WEIGHT, m_staticWeightVal);
	DDX_Control(pDX, IDC_STATIC_HTHRESH, m_staticHitThreshVal);
	DDX_Control(pDX, IDC_SLIDER_WINSTRIDE, m_sliderCtrlWinStride);
	DDX_Control(pDX, IDC_SLIDER_PADDING, m_sliderCtrlPadding);
	DDX_Control(pDX, IDC_STATIC_WINSTRIDE, m_staticWinStrideVal);
	DDX_Control(pDX, IDC_STATIC_PADDING, m_staticPadding);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_editCtrlScale);
	DDX_Control(pDX, IDC_EDIT_WEIGHTS, m_editCtrlWeight);
	DDX_Control(pDX, IDC_EDIT_HTHRESH, m_editCtrlHThresh);
	DDX_Control(pDX, IDC_CHECK_OFFON, m_checkOffOn);
	DDX_Control(pDX, IDC_MFCBUTTON_PAUSEON, m_MfcButtonPauseOn);
	DDX_Control(pDX, IDC_MFCBUTTON_PAUSEOFF, m_MfcButtonPauseOff);
}

BOOL CCameraTrackerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT

	return CFormView::PreCreateWindow(cs);
}

void CCameraTrackerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_sliderCtrlScale.SetRange(CAMTRACK_DIVIDER, CAMTRACK_DIVIDER * 3, TRUE);
	m_sliderCtrlWeight.SetRange(0, CAMTRACK_DIVIDER * 2, TRUE);
	m_sliderCtrlHThresh.SetRange(0, CAMTRACK_DIVIDER * 2, TRUE);
	m_sliderCtrlWinStride.SetRange(0, 16, TRUE);
	m_sliderCtrlPadding.SetRange(0, 16, TRUE);

	m_sliderCtrlScale.SetPos(CAMTRACK_DIVIDER * 1.05);
	m_sliderCtrlWeight.SetPos(0);
	m_sliderCtrlHThresh.SetPos(0);

	m_staticScaleVal.SetWindowTextW(GetLabelStr(IDS_SCALE));
	m_staticWeightVal.SetWindowTextW(GetLabelStr(IDS_WEIGHT));
	m_staticWeightVal.SetWindowTextW(GetLabelStr(IDS_HITTHRESH));
}

void CCameraTrackerView::OnDraw(CDC* pDC)
{
	CCameraTrackerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	CWnd *dlgItem = GetDlgItem(IDC_STATIC_PIC);
	dlgItem->GetWindowRect(rect);

	cv::destroyAllWindows();
	cvNamedWindow("IDC_STATIC_OUTPUT");
	cvResizeWindow("IDC_STATIC_OUTPUT", rect.Width(), rect.Height());
	HWND hWnd = (HWND)cvGetWindowHandle("IDC_STATIC_OUTPUT");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

}

// Diagnostyka klasy CCameraTrackerView

#ifdef _DEBUG
void CCameraTrackerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCameraTrackerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCameraTrackerDoc* CCameraTrackerView::GetDocument() const // wbudowana jest wersja bez debugowania
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCameraTrackerDoc)));
	return (CCameraTrackerDoc*)m_pDocument;
}
#endif //_DEBUG


// Procedury obsługi komunikatów CCameraTrackerView


void CCameraTrackerView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		SetCEdit(m_editCtrlScale, m_sliderCtrlScale);
		SetCEdit(m_editCtrlWeight, m_sliderCtrlWeight);
		SetCEdit(m_editCtrlHThresh, m_sliderCtrlHThresh);
		UpdateValues();
	}

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCameraTrackerView::OnEnKillfocusEdit()
{
	SetCSlider(m_sliderCtrlScale, m_editCtrlScale);
	SetCSlider(m_sliderCtrlWeight, m_editCtrlWeight);
	SetCSlider(m_sliderCtrlHThresh, m_editCtrlHThresh);
	UpdateValues();
}

void CCameraTrackerView::OnEnUpdateEditHthresh()
{

}


// Metody pomocnicze


CString CCameraTrackerView::GetLabelStr(UINT ids, int value)
{
	CString valString;
	CString idsString;
	idsString.LoadStringW(ids);

	if (value == -1)
		return idsString;

	switch (ids)
	{
	default:
		valString.Format(_T(": %d"), value);
		break;
	}

	return idsString + valString;
}

void CCameraTrackerView::UpdateValues()
{
	CCameraTrackerDoc* pDoc = GetDocument();
	double scale = (double)m_sliderCtrlScale.GetPos() / CAMTRACK_DIVIDER;
	double weight = (double)m_sliderCtrlWeight.GetPos() / CAMTRACK_DIVIDER;
	double hitThresh = (double)m_sliderCtrlHThresh.GetPos() / CAMTRACK_DIVIDER;
	int winStride = m_sliderCtrlWinStride.GetPos();
	int padding = m_sliderCtrlPadding.GetPos();

	m_staticWeightVal.SetWindowTextW(GetLabelStr(IDS_WINSTRIDE, winStride));
	m_staticPadding.SetWindowTextW(GetLabelStr(IDS_Padding, padding));
	pDoc->SetAnalyzerAttr(scale, weight, hitThresh, winStride, padding);
}

bool CCameraTrackerView::SetCSlider(CSliderCtrl& cslider, CEdit& cedit)
{
	CString cstr;
	cedit.GetWindowTextW(cstr);
	double val = _tstof(cstr) * CAMTRACK_DIVIDER;
	cslider.SetPos((int)val);
}

bool CCameraTrackerView::SetCEdit(CEdit& cedit, CSliderCtrl& cslider)
{
	double val = (double)cslider.GetPos() * CAMTRACK_DIVIDER;
	CString str;
	str.Format(_T("%f"), val);
	cedit.SetWindowTextW(str);
}

bool CCameraTrackerView::ValidateCEditInput(CEdit& cedit)
{
	CString cstr;
	cedit.GetWindowTextW(cstr);
	return ValidateCEditInput(cstr);
}

bool CCameraTrackerView::ValidateCEditInput(CString cstr)
{
	if (cstr.IsEmpty()) return false;
	return cstr.SpanIncluding(_T(".0123456789")) == cstr;
}
