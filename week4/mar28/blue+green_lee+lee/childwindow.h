#include<windows.h>
#ifndef _childwindow_h_
#define _childwindow_h_
class ChildWindow
{
public:
    ChildWindow()=delete; // �⺻ ������ ����.
    ChildWindow(OPENFILENAME _OFN);
    OPENFILENAME* get_openfilename(); // getter
    void set_window();
    // �Ľ��ϴ� �Լ��߰�.
private:
    OPENFILENAME OFN; // childwindow�� �ε��ؾ��ϴ� ������ ������ ����ִ� ����ü.
    HINSTANCE hinst; // ���μ����� �ڵ�
    WNDCLASS win; // window ����ü
    HWND hwnd;    // window�� �����Ǹ�, �ڵ��� ���⿡ �����Ѵ�.

    /*

        obj���� �ҷ��� �������� �����ؾ���..

                                */
};
#endif
