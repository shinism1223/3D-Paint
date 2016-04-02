#include "childwindow.h"
#include "header.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;


// child window message call back function
LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    Mesh *mesh = window_main.get_child_window()->getMesh();

    // OpenGL display function in mesh object
    mesh->display();

    PAINTSTRUCT ps;
    switch(iMessage)
    {
    }
    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}



ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;
    set_window();

    //파싱해서 정보들을 저장하는 함수 호출 해야되는 부분
    // set .obj file path in parser
    parser.setPath(OFN.lpstrFile);
    // set mesh
    parser.setMesh(&mesh);
    // parse .obj file to mesh data format
    parser.parse();

    glContext.init(hwnd);

    initGL();
}


OPENFILENAME* ChildWindow::get_openfilename()
{
    return &OFN;
}
void ChildWindow::set_window()
{
    win.cbClsExtra=0;
    win.cbWndExtra=0;
    win.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    win.hInstance=g_hInst;
    win.lpfnWndProc=(WNDPROC)ChildWndProc;
    win.lpszMenuName=MAKEINTRESOURCE(NULL);
    win.lpszClassName="Child";
    win.style=CS_HREDRAW|CS_VREDRAW;
    RegisterClass(&win);
    hwnd = CreateWindowEx(0,"Child","Child",WS_CHILD|WS_VISIBLE|WS_SYSMENU|WS_CLIPCHILDREN ,2,30,980,650,window_main.get_handle(),(HMENU)NULL,g_hInst,NULL);
}

Mesh* ChildWindow::getMesh()
{
    return &mesh;
}

Parser* ChildWindow::getParser()
{
    return &parser;
}

GLContext* ChildWindow::getGLContext()
{
    return &glContext;
}

HWND ChildWindow::getHWND()
{
    return hwnd;
}

void
ChildWindow::initGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
}
