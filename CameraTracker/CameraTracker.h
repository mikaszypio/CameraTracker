
// CameraTracker.h: główny plik nagłówkowy aplikacji CameraTracker
//
#pragma once

#ifndef __AFXWIN_H__
	#error "dołącz nagłówek „pch.h” przed dołączeniem tego pliku na potrzeby optymalizacji PCH"
#endif

#include "resource.h"       // główne symbole


// CCameraTrackerApp:
// Aby uzyskać implementację klasy, zobacz CameraTracker.cpp
//

class CCameraTrackerApp : public CWinApp
{
public:
	CCameraTrackerApp() noexcept;


// Przesłania
public:
	virtual BOOL InitInstance();

// Implementacja
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCameraTrackerApp theApp;
