#ifndef __MAIN_H__
#define __MAIN_H__

#include<vector>
using namespace std;

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"tinyfiledialogs.h"

#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<windows.h>
#include<GL/glut.h>
#endif

#define KIST_LEFT_BUTTON    0x0010
#define KIST_RIGHT_BUTTON   0x0020
#define KIST_BOTH_BUTTON    0x0030
#define KIST_SCROLL_UP      0x0003
#define KIST_SCROLL_DOWN    0x0004

struct Vertex
{
    Vertex(float x=0,float y=0,float z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Vertex operator-(Vertex v)
    {
        Vertex r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
        return r;
    }
    Vertex operator/(GLfloat d)
    {
        Vertex r;
        r.x=x/d;
        r.y=y/d;
        r.z=z/d;
        return r;
    }
    GLfloat getLength()
    {
        return sqrt(x*x+y*y+z*z);
    }
    float x,y,z;
};

struct Face
{
    vector<int> vertexNo;
};

vector<Vertex> aVertex;
vector<Face> aFace;

const int WindowWidth=600;
const int WindowHeight=600;

inline void glVertex3fv(Vertex &v)
{
    glVertex3f(v.x,v.y,v.z);
}

inline int glConvertButton(int button)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        return KIST_LEFT_BUTTON;
    case GLUT_RIGHT_BUTTON:
        return KIST_RIGHT_BUTTON;
    default:
        return button;
    }
}

#endif // __MAIN_H__
