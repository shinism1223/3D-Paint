#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED

#include <windows.h>

class GLContext{
public:

    GLContext(){
        reset();
    }

    ~GLContext(){
        reset();
    }

    void init(HWND hWnd);
    void purge();
    HDC getHDC();

private:

    void reset();

    HWND mhWnd;
    HDC mhDC;
    HGLRC mhRC;
};

#endif // GLCONTEXT_H_INCLUDED
