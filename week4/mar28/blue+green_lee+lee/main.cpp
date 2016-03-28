#define _WIN32_WINNT 0x0500

#include "resource/resource.h"
#include "mainwindow.h"
#include "childwindow.h"
#include "header.h"

MainWindow window_main; // 윈도우 인스턴스의 참조자
HINSTANCE g_hInst; // 프로세스의 handle을 저장할 변수

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow) // 메인함수.
{

//    ShowWindow(GetConsoleWindow(),SW_HIDE); // 콘솔창이 사라지도록  하는 함수이다.

    g_hInst=hInstance;
    MSG Message;

    window_main=MainWindow(hInstance,"3DPaint",IDR_MENU1); // MainWindow 인스턴스 생성

    window_main.show_window(50,50,1000,700); // window 출력

    while(GetMessage(&Message,0,0,0))  // 메세지처리
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return 0;
}
