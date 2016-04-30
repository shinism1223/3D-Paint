#include "mesh.h"
#include <GL/gl.h>
#include <stdio.h>
#include "header.h"

// OpenGL display function
void Mesh::display()
{
    //TODO : draw mesh
    //puts("i will draw");
    glClear(GL_COLOR_BUFFER_BIT);

    float scaling = 1.0;
    glColor3f(1.0, 0.0, 0.0);
    for(const TRIANGLE& t : triangle){
        glBegin(GL_TRIANGLES);
            //printf("%f, %f, %f\n", t.v[0].x, t.v[0].y, t.v[0].z);
            //printf("%f, %f, %f\n", t.v[1].x, t.v[1].y, t.v[1].z);
            //printf("%f, %f, %f\n", t.v[2].x, t.v[2].y, t.v[2].z);
            glVertex3f(t.v[0].x * scaling, t.v[0].y * scaling, t.v[0].z* scaling);
            glVertex3f(t.v[1].x * scaling, t.v[1].y * scaling, t.v[1].z* scaling);
            glVertex3f(t.v[2].x * scaling, t.v[2].y * scaling, t.v[2].z* scaling);
        glEnd();
    }

    glFlush();
    //printf("%d\n", mesh->triangle.size());
    /*
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(-0.2, -0.2, 0.0);
        glVertex3f(0.2, -0.2, 0.0);
        glVertex3f(0.0, 0.2, 0.0);
    glEnd();

    glFlush();
    */
    // swap buffer
    SwapBuffers(/*HWND of child window*/window_main.get_child_window()->getGLContext()->getHDC());
}
