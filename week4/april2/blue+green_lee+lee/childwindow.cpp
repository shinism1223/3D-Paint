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

bool bEnableOpenGL = false;

// child window message call back function
LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT ps;
    switch(iMessage)
    {
    case WM_PAINT:

        break;

    // TODO: Mouse click event message receiving
    // TODO: Mouse moving event message receiving
    // TODO: Keyboard key-down event message receiving

    default:

    // TODO: should move display() from here to appropriate places in this callback function
        // Check whether ready to draw mesh
        if(bEnableOpenGL){
        // Get mesh data
        Mesh *mesh = window_main.get_child_window()->getMesh();

        // Display mesh data
        mesh->display();
        }
        break;
    }
    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}



ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;

    set_window();

    // Set .obj file path in parser object
    parser.setPath(OFN.lpstrFile);
    // Set mesh
    parser.setMesh(&mesh);
    // Parse .obj file to mesh data format
    parser.parse();

    // Device Context, Render Context Initialization
    glContext.init(hwnd);

    // Initialize OpenGL settings
    initGL();

    // From here, you can use openGL.
    bEnableOpenGL = true;
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
