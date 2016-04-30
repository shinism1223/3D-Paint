#ifndef __MAIN_H__
#define __MAIN_H__

#include<vector>
using namespace std;

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<map>
#include"tinyfiledialogs.h"

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

#define KIST_CAMERA_MODE 0
#define KIST_SELECT_MODE 1

#define KIST_LEFT_BUTTON    0x0010
#define KIST_RIGHT_BUTTON   0x0020
#define KIST_BOTH_BUTTON    0x0030
#define KIST_SCROLL_UP      0x0003
#define KIST_SCROLL_DOWN    0x0004

const int WindowWidth=600;
const int WindowHeight=600;

const char *dialogTitle="Load";
const char *filterPatterns[]={"*.obj"};
const char *filterDescription="Object files";

struct Vector3
{
    Vector3(float x=0,float y=0,float z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    static int test() {
        return 1;
    }
    Vector3 operator*(float k) {
        Vector3 r;
        r.x = x * k;
        r.y = y * k;
        r.z = z * k;
        return r;
    }
    
    Vector3 operator-()
    {
        Vector3 r;
        r.x=-x;
        r.y=-y;
        r.z=-z;
        return r;
    }
    Vector3 operator-(Vector3 v)
    {
        Vector3 r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
        return r;
    }
    Vector3 operator+(Vector3 v)
    {
        Vector3 r;
        r.x=x+v.x;
        r.y=y+v.y;
        r.z=z+v.z;
        return r;
    }
    Vector3 operator/(GLfloat d)
    {
        Vector3 r;
        r.x=x/d;
        r.y=y/d;
        r.z=z/d;
        return r;
    }
    GLfloat getNorm()
    {
        return sqrt(x*x+y*y+z*z);
    }
    Vector3 UnitVector()
    {
        return (*this) * (1. / getNorm());
    }
    void PrintValue() {
        printf("[%lf %lf %lf]\n", x, y, z);
    }
    float x,y,z;
};

struct CAMERA{
    Vector3 position, normal, yAxis, xAxis;
    Vector3 baseNormal, baseYAxis, baseXAxis;
    float azimuth, elevation, twist;
    CAMERA() {
        position = Vector3(0,0,0);
        baseNormal = normal = Vector3(0,0,-1);
        baseXAxis = xAxis = Vector3(1,0,0);
        baseYAxis = yAxis = Vector3(0,1,0);
        azimuth = elevation = twist = 0;
    }
}Camera;

struct Face
{
    vector<int> vertexNo;
    vector<Vector3> normal;
};

vector<Vector3> aVertex,aNormal;
vector<Face> aFace;

inline void glVertex3fv(Vector3 &v)
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

void drawBitmapText(char *string,float x,float y,float z)
{
    char *c;
    glRasterPos3f(x, y, z);
    
    for (c=string; *c != 0; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        /*
         GLUT_BITMAP_TIMES_ROMAN_10
         GLUT_BITMAP_TIMES_ROMAN_24
         GLUT_BITMAP_HELVETICA_10
         GLUT_BITMAP_HELVETICA_12
         GLUT_BITMAP_HELVETICA_18
        */
    }
}
void gluLookAt(Vector3 eye, Vector3 center, Vector3 up) {
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

float InnerProduct(Vector3 A, Vector3 B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}
Vector3 CrossProduct(Vector3 A, Vector3 B) {
    Vector3 r;
    r.x = A.y * B.z - A.z * B.y;
    r.y = A.z * B.x - A.x * B.z;
    r.z = A.x * B.y - A.y * B.x;
    return r;
}

Vector3 rotateVector(Vector3 target, Vector3 axis, float theta) {
    Vector3 parallel = axis * (InnerProduct(target, axis) / InnerProduct(axis, axis));
    Vector3 vertical = target - parallel;
    Vector3 cross = CrossProduct(axis, vertical);
    
    Vector3 result = (vertical.UnitVector() * cos(theta) + cross.UnitVector() * sin(theta)) * vertical.getNorm() + parallel;
    return result;
}

#endif // __MAIN_H__









