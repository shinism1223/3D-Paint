#include<windows.h>
#ifndef _childwindow_h_
#define _childwindow_h_
class ChildWindow
{
public:
    ChildWindow()=delete; // 기본 생성자 삭제.
    ChildWindow(OPENFILENAME _OFN);
    OPENFILENAME* get_openfilename(); // getter
    void set_window();
    // 파싱하는 함수추가.
private:
    OPENFILENAME OFN; // childwindow가 로드해야하는 파일의 정보가 담겨있는 구조체.
    HINSTANCE hinst; // 프로세스의 핸들
    WNDCLASS win; // window 구조체
    HWND hwnd;    // window가 생성되면, 핸들을 여기에 저장한다.

    /*

        obj에서 불러온 정보들을 저장해야함..

                                */
};
#endif
