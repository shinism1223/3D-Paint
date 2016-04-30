#include<windows.h>
#include<commctrl.h>
#include "header.h"
#include "mainwindow.h"
#include "childwindow.h"

HWND hToolBar; // ������ �ڵ�
OPENFILENAME OFN; // �ε��ϴ� ������ ������ �����ϴ� ����ü.

char str[300]; // ����ü�� �ʿ��� ������
char lpstrFile[MAX_PATH]="";

TBBUTTON ToolBtn[5]={ // ���� ����.
   {0,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
   {1,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
   {5,0,0,TBSTYLE_SEP,0,0,0,0},
   {2,12,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,0},
   {3,13,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,0}
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam) // ������ ���ν���
{
    int a;
    switch(iMessage)
    {
    case WM_CREATE:
        InitCommonControls();
        hToolBar=CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE, 103, 4, g_hInst,IDB_BITMAP1, ToolBtn, 5, 16,16,16,16,sizeof(TBBUTTON));
        // ���ٸ� �����Ѵ�.
        return 0;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case 10: // ���� ���� ����ư�� �ڵ�.
        case IDR_LOAD: // or FILE->LOAD�� ����������
            memset(&OFN,0,sizeof(OPENFILENAME));
            OFN.lStructSize=sizeof(OPENFILENAME);
            OFN.hwndOwner=hWnd;
            OFN.lpstrFile=lpstrFile;
            OFN.nMaxFile=256;
            OFN.lpstrInitialDir="c:\\";

            if (GetOpenFileName(&OFN)!=0) {
                window_main.set_child_window(OFN); // OFN�� ���� childWindow�ν��Ͻ� ����
            }
            break;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        SendMessage(hToolBar,TB_AUTOSIZE,0,0); // ���� ������ ����
        return 0;
    }
    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}
MainWindow::MainWindow(){}

ChildWindow* MainWindow::get_child_window()
{
    return child;
}

MainWindow::MainWindow(HINSTANCE hInstance,LPSTR lpszClassName,const int menu)
{
    hinst=hInstance;
    win.cbClsExtra=0;
    win.cbWndExtra=0;
    win.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    win.hInstance=hInstance;
    win.lpfnWndProc=(WNDPROC)WndProc;
    win.lpszMenuName=MAKEINTRESOURCE(menu);
    win.lpszClassName=lpszClassName;
    win.style=CS_HREDRAW|CS_VREDRAW;
    RegisterClass(&win);
}
void MainWindow::show_window(int x,int y,int h,int w)
{
    hwnd=CreateWindowEx(WS_EX_RIGHT,win.lpszClassName,win.lpszClassName,WS_OVERLAPPEDWINDOW,
                      x,y,h,w,
                      NULL,(HMENU)NULL,hinst,NULL);
    ShowWindow(hwnd,10);
}

void MainWindow::set_child_window(OPENFILENAME _OFN){
    this->child=new ChildWindow(_OFN);
}

HWND MainWindow::get_handle(){
    return hwnd;
}
