#define _WIN32_WINNT 0x0500

#include "resource/resource.h"
#include "mainwindow.h"
#include "childwindow.h"
#include "header.h"

MainWindow window_main; // ������ �ν��Ͻ��� ������
HINSTANCE g_hInst; // ���μ����� handle�� ������ ����

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow) // �����Լ�.
{

//    ShowWindow(GetConsoleWindow(),SW_HIDE); // �ܼ�â�� ���������  �ϴ� �Լ��̴�.

    g_hInst=hInstance;
    MSG Message;

    window_main=MainWindow(hInstance,"3DPaint",IDR_MENU1); // MainWindow �ν��Ͻ� ����

    window_main.show_window(50,50,1000,700); // window ���

    while(GetMessage(&Message,0,0,0))  // �޼���ó��
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return 0;
}
