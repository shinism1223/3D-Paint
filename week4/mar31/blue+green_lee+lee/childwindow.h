#include <windows.h>
#ifndef _childwindow_h_
#define _childwindow_h_

#include "mesh.h"
#include "parser.h"
#include "glcontext.h"

class ChildWindow
{
public:
    ChildWindow()=delete; // 기본 생성자 삭제.
    ChildWindow(OPENFILENAME _OFN);
    OPENFILENAME* get_openfilename(); // getter
    void set_window();
    Mesh* getMesh();                // getter of Mesh
    Parser* getParser();            // getter of Parser
    GLContext* getGLContext();      // getter of GLContext
    HWND getHWND();                 // getter of HWND
    void initGL();

private:
    OPENFILENAME OFN; // childwindow가 로드해야하는 파일의 정보가 담겨있는 구조체.
    HINSTANCE hinst; // 프로세스의 핸들
    WNDCLASS win; // window 구조체
    HWND hwnd;    // window가 생성되면, 핸들을 여기에 저장한다.
    Mesh mesh;              // mesh data
    Parser parser;          // parser (.obj file -> mesh data)
    GLContext glContext;    // glContext setting - Enable OpenGL in Win32
};
#endif
