
// CameraTracker.cpp: definiuje zachowania klasy dla aplikacji.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CameraTracker.h"
#include "MainFrm.h"

#include "CameraTrackerDoc.h"
#include "CameraTrackerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCameraTrackerApp

BEGIN_MESSAGE_MAP(CCameraTrackerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCameraTrackerApp::OnAppAbout)
	// Standardowe polecenia dokumentu oparte na pliku
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Konstrukcja klasy CCameraTrackerApp

CCameraTrackerApp::CCameraTrackerApp() noexcept
{
	// TODO: Zastąp ciąg identyfikatora aplikacji poniżej za pomocą unikatowego ciągu identyfikatora; zalecane
	// format ciągu to CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UG.CameraTracker.0"));

	// TODO: W tym miejscu dodaj kod konstruktora,
	// Umieść wszystkie znaczące inicjacje w InitInstance
}

// Jedyny obiekt CCameraTrackerApp

CCameraTrackerApp theApp;


// Inicjowanie klasy CCameraTrackerApp

BOOL CCameraTrackerApp::InitInstance()
{
	// Funkcja InitCommonControlsEx() jest wymagana w systemie Windows XP, jeśli aplikacja
	// manifest określa użycie pliku ComCtl32.dll w wersji 6 lub nowszej, który ma zostać użyty
	// style wizualne. W przeciwnym razie utworzenie jakiegokolwiek okna nie powiedzie się.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Ustaw ten element tak, aby uwzględniał wszystkie typowe klasy kontrolek, których chcesz użyć
	// w aplikacji.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// Funkcja AfxInitRichEdit2() musi używać kontrolki RichEdit
	// AfxInitRichEdit2();

	// Inicjowanie standardowe
	// Jeśli nie używasz tych funkcji i chcesz zmniejszyć rozmiar
	// końcowego pliku wykonywalnego, usuń poniżej
	// określone procedury inicjowania, które nie są potrzebne
	// Zmień klucz rejestru, w którym są przechowywane ustawienia
	// TODO: zmodyfikuj ten ciąg, aby był poprawny
	// takie jak nazwa firmy lub organizacji
	SetRegistryKey(_T("Aplikacje lokalne wygenerowane przez kreatora aplikacji"));
	LoadStdProfileSettings(4);  // Ładuj opcje ze standardowego pliku INI (włącznie z listą ostatnio używanych)


	// Rejestruj szablony dokumentu aplikacji. Szablony dokumentu
	//  służą jako połączenie między dokumentami, oknami ramek i widokami
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCameraTrackerDoc),
		RUNTIME_CLASS(CMainFrame),       // główne okno ramek SDI
		RUNTIME_CLASS(CCameraTrackerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Analizuj wiersz polecenia pod kątem standardowych poleceń powłoki, DDE i otwarcia pliku
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Włącz otwieranie za pomocą polecenia Execute DDE
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Polecenia wysyłania określone w wierszu polecenia. Zwraca wartość FALSE, jeśli
	// aplikacja została uruchomiona z parametrem /RegServer, /Register, /Unregserver lub /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Jedyne okno zostało zainicjowane, więc wyświetl je i zaktualizuj
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// wywołaj element DragAcceptFiles tylko wtedy, gdy występuje sufiks
	//  W aplikacji SDI ten element powinien występować po elemencie ProcessShellCommand
	// Włącz otwieranie w trybie przeciągnij i upuść
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// procedury obsługi komunikatów klasy CCameraTrackerApp


// Okno dialogowe CAboutDlg używane na potrzeby informacji o aplikacji

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

// Implementacja
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Polecenie aplikacji uruchamiające okno dialogowe
void CCameraTrackerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// procedury obsługi komunikatów klasy CCameraTrackerApp



