#include "mesh.h"
#include <GL/gl.h>
#include <stdio.h>
#include "header.h"

// OpenGL display function
void Mesh::display()
{
    /* TODO : draw mesh
    //puts("i will draw");
    if(flag){

        for(const TRIANGLE& triangle : mesh->triangle){
            //glBegin(GL_TRIANGLES);
                printf("%f, %f, %f\n", triangle.v[0].x, triangle.v[0].y, triangle.v[0].z);
                printf("%f, %f, %f\n", triangle.v[1].x, triangle.v[1].y, triangle.v[1].z);
                printf("%f, %f, %f\n", triangle.v[2].x, triangle.v[2].y, triangle.v[2].z);
                //glVertex3f(triangle.v[0].x, triangle.v[0].y, triangle.v[0].z);
                //glVertex3f(triangle.v[1].x, triangle.v[1].y, triangle.v[1].z);
                //glVertex3f(triangle.v[2].x, triangle.v[2].y, triangle.v[2].z);
            //glEnd();
        }
        printf("%d\n", mesh->triangle.size());
        flag = 0;
    }*/
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(-0.2, -0.2, 0.0);
        glVertex3f(0.2, -0.2, 0.0);
        glVertex3f(0.0, 0.2, 0.0);
    glEnd();

    glFlush();

    // swap buffer
    SwapBuffers(/*HWND of child window*/window_main.get_child_window()->getGLContext()->getHDC());
}
