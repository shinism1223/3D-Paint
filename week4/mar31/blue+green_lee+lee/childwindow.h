#include <windows.h>
#ifndef _childwindow_h_
#define _childwindow_h_

#include "mesh.h"
#include "parser.h"
#include "glcontext.h"

class ChildWindow
{
public:
    ChildWindow()=delete; // �⺻ ������ ����.
    ChildWindow(OPENFILENAME _OFN);
    OPENFILENAME* get_openfilename(); // getter
    void set_window();
    Mesh* getMesh();                // getter of Mesh
    Parser* getParser();            // getter of Parser
    GLContext* getGLContext();      // getter of GLContext
    HWND getHWND();                 // getter of HWND
    void initGL();

private:
    OPENFILENAME OFN; // childwindow�� �ε��ؾ��ϴ� ������ ������ ����ִ� ����ü.
    HINSTANCE hinst; // ���μ����� �ڵ�
    WNDCLASS win; // window ����ü
    HWND hwnd;    // window�� �����Ǹ�, �ڵ��� ���⿡ �����Ѵ�.
    Mesh mesh;              // mesh data
    Parser parser;          // parser (.obj file -> mesh data)
    GLContext glContext;    // glContext setting - Enable OpenGL in Win32
};
#endif
