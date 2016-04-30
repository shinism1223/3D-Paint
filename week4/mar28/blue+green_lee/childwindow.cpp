#include "childwindow.h"
#include "header.h"
#include<windows.h>
#include<stdio.h>

using namespace std;
ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;
    set_window();
    //�Ľ��ؼ� �������� �����ϴ� �Լ� ȣ�� �ؾߵǴ� �κ�
}
LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT ps;
    switch(iMessage)
    {
    case WM_MOVE:
        RECT r,r2;
        GetWindowRect(hWnd,&r);
        GetWindowRect(window_main.get_handle(),&r2);
        if(r.left<r2.left)MoveWindow(hWnd,0,r.top-r2.top-50,700,500,true);  // �ڽ� �����찡 ��,�������� ������ �ʵ���..!�ٺ����� �ذ�å�� �ƴѰͰ���.. �߿��Ѱ� �ƴϴ� �������θ�..!
        else if(r.top<r2.top+80)MoveWindow(hWnd,r.left-r2.left-10,30,700,500,true);
    case WM_PAINT: // ������۸�     ��� http://hardprogram.tistory.com/entry/API-������۸�-��� �����ߴ�.
        RECT crt;
        GetClientRect(hWnd, &crt);
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
        HBITMAP OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        BitBlt(hdc, 0,0, crt.right, crt.bottom, hMemDC, 0,0, SRCCOPY);
        DeleteObject(SelectObject(hMemDC, OldBitmap));
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
        return 0;
    }
    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}
OPENFILENAME* ChildWindow::get_openfilename()
{
    return &OFN;
}
void ChildWindow::set_window()
{
    win.cbClsExtra=0;
    win.cbWndExtra=0;
    win.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    win.hInstance=g_hInst;
    win.lpfnWndProc=(WNDPROC)ChildWndProc;
    win.lpszMenuName=MAKEINTRESOURCE(NULL);
    win.lpszClassName="Child";
    win.style=CS_HREDRAW|CS_VREDRAW;
    RegisterClass(&win);
    CreateWindowEx(0,"Child","Child",WS_CHILD|WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_THICKFRAME |WS_CLIPCHILDREN ,10,30,700,500,window_main.get_handle(),(HMENU)NULL,g_hInst,NULL);
}
