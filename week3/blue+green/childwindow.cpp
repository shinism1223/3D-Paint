#include "childwindow.h"
#include "header.h"
#include<windows.h>
#include<stdio.h>

using namespace std;
ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;
    set_window();
    //파싱해서 정보들을 저장하는 함수 호출 해야되는 부분
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
        if(r.left<r2.left)MoveWindow(hWnd,0,r.top-r2.top-50,700,500,true);  // 자식 윈도우가 위,왼쪽으로 나가지 않도록..!근본적인 해결책은 아닌것같다.. 중요한건 아니니 이정도로만..!
        else if(r.top<r2.top+80)MoveWindow(hWnd,r.left-r2.left-10,30,700,500,true);
    case WM_PAINT: // 더블버퍼링     기법 http://hardprogram.tistory.com/entry/API-더블버퍼링-기법 참고했당.
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
