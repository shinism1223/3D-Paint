#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/gl.h>
#include <vector>

struct VECTOR2
{
    GLfloat x, y;
};

struct VECTOR3
{
    GLfloat x, y, z;
};

struct POLYGON
{
    std::vector<int> vIndex;
    std::vector<int> vtIndex;
    std::vector<int> vnIndex;
    /*
    std::vector<VECTOR3> v;
    std::vector<VECTOR2> vt;
    std::vector<VECTOR3> vn;
    */
};

struct TRIANGLE
{
    VECTOR3 v[3];
    VECTOR2 vt[3];//? : What value should be default value?
    VECTOR3 vn[3];//? : What value should be defalut value?
};

class Mesh
{
public:
    void display();

public:
    std::vector<VECTOR3> v;
    std::vector<VECTOR2> vt;
    std::vector<VECTOR3> vn;
    std::vector<TRIANGLE> triangle;
    std::vector<POLYGON> polygon;
};





#endif // MESH_H_INCLUDED
