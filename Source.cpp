#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "procs.h"
#include "funcs.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevhInstance,PSTR cmdLine, int ShowCmd){
	MSG msg;
	HWND hLog;

	//
	hLog = CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_LOGIN),NULL,(DLGPROC)LogProc,(LPARAM)hInstance);
	ShowWindow(hLog,ShowCmd);

	ZeroMemory(&msg, sizeof(MSG));
	while(GetMessage(&msg,NULL,0,0)){
		if( hLog == 0 || !IsDialogMessage(hLog, &msg ) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

