
// CameraTrackerView.cpp: implementacja klasy CCameraTrackerView
//

#include "pch.h"
#include "framework.h"
#include "CameraTracker.h"
#include "CameraTrackerDoc.h"
#include "CameraTrackerView.h"
#include <opencv2/highgui/highgui_c.h>
#include <cctype>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CAMTRACKVIEW_DIVIDER 1000000
#define CAMTRACKVIEW_FCHARS ".01234567890"

// CCameraTrackerView

IMPLEMENT_DYNCREATE(CCameraTrackerView, CFormView)

BEGIN_MESSAGE_MAP(CCameraTrackerView, CFormView)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_WEIGHTS, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_HTHRESH, &CCameraTrackerView::OnEnKillfocusEdit)
	ON_EN_UPDATE(IDC_EDIT_HTHRESH, &CCameraTrackerView::OnEnUpdateEditHthresh)
	ON_EN_UPDATE(IDC_EDIT_SCALE, &CCameraTrackerView::OnEnUpdateEditScale)
	ON_EN_UPDATE(IDC_EDIT_WEIGHTS, &CCameraTrackerView::OnEnUpdateEditWeights)
	ON_BN_CLICKED(IDC_CHECK_OFFON, &CCameraTrackerView::OnBnClickedCheckOffOn)
	ON_BN_CLICKED(IDC_BUTTON_PAUSEON, &CCameraTrackerView::OnBnClickedButtonPauseon)
	ON_BN_CLICKED(IDC_BUTTON_PAUSEOFF, &CCameraTrackerView::OnBnClickedButtonPauseoff)
	ON_BN_CLICKED(IDC_CHECK_GROUPING, &CCameraTrackerView::OnBnClickedCheckGrouping)
	ON_STN_CLICKED(IDC_STATIC_PIC, &CCameraTrackerView::OnStnClickedStaticPic)
	ON_COMMAND(ID_FILE_SAVE_BEGIN, &CCameraTrackerView::OnFileSaveBegin)
	ON_COMMAND(ID_FILE_SAVE_FIN, &CCameraTrackerView::OnFileSaveFin)
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
	DDX_Control(pDX, IDC_STATIC_PIC, m_staticPic);
	DDX_Control(pDX, IDC_CHECK_GROUPING, m_checkGrouping);
	DDX_Control(pDX, IDC_BUTTON_PAUSEON, m_buttonPauseOn);
	DDX_Control(pDX, IDC_BUTTON_PAUSEOFF, m_buttonPauseOff);
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

	m_sliderCtrlScale.SetRange(CAMTRACKVIEW_DIVIDER, CAMTRACKVIEW_DIVIDER * 3, TRUE);
	m_sliderCtrlWeight.SetRange(0, CAMTRACKVIEW_DIVIDER * 2, TRUE);
	m_sliderCtrlHThresh.SetRange(0, CAMTRACKVIEW_DIVIDER * 2, TRUE);
	m_sliderCtrlWinStride.SetRange(0, 16, TRUE);
	m_sliderCtrlPadding.SetRange(0, 16, TRUE);

	m_sliderCtrlScale.SetPos(CAMTRACKVIEW_DIVIDER * (int)1.05);
	m_sliderCtrlWeight.SetPos(0);
	m_sliderCtrlHThresh.SetPos(0);

	m_staticScaleVal.SetWindowTextW(GetLabelStr(IDS_SCALE));
	m_staticWeightVal.SetWindowTextW(GetLabelStr(IDS_WEIGHT));
	m_staticWeightVal.SetWindowTextW(GetLabelStr(IDS_HITTHRESH));

	SetCEdit(m_editCtrlScale, m_sliderCtrlScale);
	SetCEdit(m_editCtrlWeight, m_sliderCtrlWeight);
	SetCEdit(m_editCtrlHThresh, m_sliderCtrlHThresh);

	m_buttonPauseOff.EnableWindow(FALSE);
	m_checkOffOn.SetCheck(TRUE);
}

void CCameraTrackerView::OnDraw(CDC* pDC)
{
	CCameraTrackerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Na czysto:
	RECT rect;
	m_staticPic.GetClientRect(&rect);
	pDoc->SetProcPictureCtrl(&m_staticPic);
	pDoc->SetImgSize(rect.right, rect.bottom);
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

void CCameraTrackerView::OnEnUpdateEdit(CEdit& cedit)
{
	CString cstr;
	cedit.GetWindowTextW(cstr);
	if (!ValidateCEditInput(cstr))
	{
		if (cstr.IsEmpty())
			return;
		cedit.SetWindowTextW(cstr.SpanExcluding(_T(CAMTRACKVIEW_FCHARS)));
	}
}

void CCameraTrackerView::OnEnUpdateEditHthresh()
{
	OnEnUpdateEdit(m_editCtrlHThresh);
}

void CCameraTrackerView::OnEnUpdateEditScale()
{
	OnEnUpdateEdit(m_editCtrlScale);
}

void CCameraTrackerView::OnEnUpdateEditWeights()
{
	OnEnUpdateEdit(m_editCtrlWeight);
}

// Metody pomocnicze

CString CCameraTrackerView::GetLabelStr(UINT ids, int value)
{
	CString valString;
	CString idsString;
	idsString.LoadStringW(ids);

	if (value == -1)
		return idsString;

	valString.Format(_T(": %d"), value);
	return idsString + valString;
}

void CCameraTrackerView::UpdateValues()
{
	CCameraTrackerDoc* pDoc = GetDocument();
	double scale = (double)m_sliderCtrlScale.GetPos() / CAMTRACKVIEW_DIVIDER;
	double weight = (double)m_sliderCtrlWeight.GetPos() / CAMTRACKVIEW_DIVIDER;
	double hitThresh = (double)m_sliderCtrlHThresh.GetPos() / CAMTRACKVIEW_DIVIDER;
	int winStride = m_sliderCtrlWinStride.GetPos();
	int padding = m_sliderCtrlPadding.GetPos();

	m_staticWinStrideVal.SetWindowTextW(GetLabelStr(IDS_WINSTRIDE, winStride));
	m_staticPadding.SetWindowTextW(GetLabelStr(IDS_PADDING, padding));
	pDoc->SetAnalyzerAttr(scale, weight, hitThresh, winStride, padding);
}

void CCameraTrackerView::SetCSlider(CSliderCtrl& cslider, CEdit& cedit)
{
	CString cstr;
	cedit.GetWindowTextW(cstr);
	if (ValidateCEditInput(cstr))
	{
		double val = _tstof(cstr) * CAMTRACKVIEW_DIVIDER;
		cslider.SetPos((int)val);
	}
}

void CCameraTrackerView::SetCEdit(CEdit& cedit, CSliderCtrl& cslider)
{
	double val = (double)cslider.GetPos() / CAMTRACKVIEW_DIVIDER;
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
	return cstr.SpanIncluding(_T(CAMTRACKVIEW_FCHARS)) == cstr;
}

void CCameraTrackerView::OnBnClickedCheckOffOn()
{
	CCameraTrackerDoc* pDoc = GetDocument();
	pDoc->SetAnalyzerOnOff(m_checkOffOn.GetCheck());
}


void CCameraTrackerView::OnBnClickedButtonPauseon()
{
	m_buttonPauseOn.EnableWindow(FALSE);
	m_buttonPauseOff.EnableWindow();
	GetDocument()->SetPause(true);
}


void CCameraTrackerView::OnBnClickedButtonPauseoff()
{
	m_buttonPauseOff.EnableWindow(FALSE);
	m_buttonPauseOn.EnableWindow();
	GetDocument()->SetPause(false);
}


void CCameraTrackerView::OnBnClickedCheckGrouping()
{
	CCameraTrackerDoc* pDoc = GetDocument();
	pDoc->SetAnalyzerGrouping(m_checkGrouping.GetCheck());
}


void CCameraTrackerView::OnStnClickedStaticPic()
{
	POINT position;
	GetCursorPos(&position);
	RECT rect;
	m_staticPic.GetWindowRect(&rect);
	unsigned int x = position.x - rect.left;
	unsigned int y = position.y - rect.top;
	GetDocument()->SetAsMarked(x, y);
}


void CCameraTrackerView::OnFileSaveBegin()
{
	GetDocument()->frmProc->StartRecording();
}


void CCameraTrackerView::OnFileSaveFin()
{
	GetDocument()->frmProc->StopRecording();
}
