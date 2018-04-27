#include "Application.h"
#include "MainWindow.h"
#include <string>

// Disable the warnings for deprecated functions (strtok and stricmp)
#pragma warning(disable:4996)

CApplication::CApplication(HINSTANCE hInstance)
	: m_hInstance(hInstance), m_bFullScreen(false)
{
}

CApplication::~CApplication()
{
}

void CApplication::ParseCmdLine(LPSTR lpCmdLine)
{
	LPSTR lpArgument = strtok(lpCmdLine, " ");
	while (lpArgument)
	{
		if (stricmp(lpArgument, "-fullscreen") == 0)
			m_bFullScreen = true;
		lpArgument = strtok(NULL, " ");
	}
}

void CApplication::Run()
{
	CMainWindow mainWindow(1000, 800, m_bFullScreen);

	MSG Message;
	Message.message = (UINT)~WM_QUIT;
	DWORD dwNextDeadLine = GetTickCount() + FRAME_TIME;
	DWORD dwSleep = FRAME_TIME;
	bool bUpdate = false;

	// Loop until a WM_QUIT message is received
	while (Message.message != WM_QUIT)
	{
		DWORD dwResult = MsgWaitForMultipleObjectsEx(0, NULL, dwSleep, QS_ALLEVENTS, 0);
		if (dwResult != WAIT_TIMEOUT)
		{
			
			while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}

			// If the current time is close (or past) to the 
			// deadline, the application should be processed.
			if (GetTickCount() >= dwNextDeadLine - 1)
				bUpdate = true;
			else
				bUpdate = false;
		}
		else
			// On a timeout, the application should be processed.
			bUpdate = true;

		// Check if the application should be processed
		if (bUpdate)
		{
			DWORD dwCurrentTime = GetTickCount();
			// Update the main window
			mainWindow.Update(dwCurrentTime);
			// Draw the main window
			mainWindow.Draw();

			dwNextDeadLine = dwNextDeadLine + FRAME_TIME;
		}
		dwSleep = dwNextDeadLine - GetCurrentTime();
		
		if (dwSleep>FRAME_TIME)
		{
			dwSleep = FRAME_TIME;
			dwNextDeadLine = GetCurrentTime() + FRAME_TIME;
		}
	}
}
