#include "childwindow.h"

#ifndef _mainwindow_h_
#define _mainwindow_h_

class MainWindow
{

public:
    MainWindow(); // 기본 생성자.
    MainWindow(HINSTANCE hInstance,LPSTR lpszClassName,const int menu); // 프로세스의 핸들, 등록할 윈도우의 클래스이름, 메뉴의 핸들 전달하는 생성자.
    HINSTANCE get_inst(); // 프로세스의 핸들을 다른 클래스에게 보낼 수 있도록 만든 메쏘드
    void show_window(const int x,const int y,const int w,const int h); // 윈도우를 띄우는 함수. x,y좌표와 w,h를 인자로 받는다.
    void set_child_window(OPENFILENAME OFN);  // OPENFILENAME을 갖는 childWindow인스턴스를 만들고 포인터가 그 인스턴스를 가르키도록 해준다.
    HWND get_handle(); // 윈도우의 핸들을 반환
    ChildWindow* get_child_window(); // child포인터를 반환


private:
    HINSTANCE hinst; // 프로세스의 핸들
    WNDCLASS win; // window 구조체
    HWND hwnd;    // window가 생성되면, 핸들을 여기에 저장한다.
    ChildWindow *child; // childwindow를 가르키는 포인터.
};

#endif
