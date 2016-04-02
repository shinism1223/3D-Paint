//
//  main.cpp
//  Viewing
//
//  Created by KJBS2 on 3/26/16.
//  Copyright (c) 2016 KJBS2. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

const int   WindowWidth       = 600;
const int   WindowHeight      = 600;
const int   WindowPositionX   = 800;
const int   WindowPositionY   = 000;
const GLfloat XcoordinateMin    = -1.0;
const GLfloat XcoordinateMax    = +1.0;
const GLfloat YcoordinateMin    = -1.0;
const GLfloat YcoordinateMax    = +1.0;
const GLfloat ZcoordinateMin    = -1.0;
const GLfloat ZcoordinateMax    = +15.0;

bool changeProjection = false;

using namespace std;

GLfloat twist, elevation, azimuth;
GLfloat roll, pitch, yaw;
GLfloat step = 10;

GLfloat moveX, moveY, moveZ=-2;
GLfloat stepMove = 0.5;

void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist) {
//    glTranslatef(0.0, 0.0, -radius);
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(-twist,       0.0, 0.0, 1.0);
    glRotatef(-elevation,   1.0, 0.0, 0.0);
    glRotatef(-azimuth,     0.0, 1.0, 0.0);
}
void PilotView(GLfloat roll, GLfloat pitch, GLfloat yaw) {
    glRotatef(-roll,    0.0, 0.0, 1.0);
    glRotatef(-pitch,   1.0, 0.0, 0.0);
    glRotatef(-yaw,     0.0, 1.0, 0.0);
}
void DoKeyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'a': azimuth   += step; break;
        case 'd': azimuth   -= step; break;
        case 'w': elevation += step; break;
        case 's': elevation -= step; break;
        case 'q': twist     -= step; break;
        case 'e': twist     += step; break;
        case 'z': azimuth = elevation = twist = 0; break;
        case 'k': changeProjection = !changeProjection; break;
        case 'n': moveZ += stepMove; break;
        case 'm': moveZ -= stepMove; break;
    }
    if(moveZ == -1.0) moveZ = -1.5;
    glutPostRedisplay();
}
void DoReshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(changeProjection)
        glOrtho(XcoordinateMin, XcoordinateMax, YcoordinateMin, YcoordinateMax, ZcoordinateMin, ZcoordinateMax);
    else
        glFrustum(-1, 1, -1, 1, 1, 15);
}

int X, Y, ButtonState = -1;
void DoMouseClick(int button, int state, int x, int y) {
    if(button == GLUT_RIGHT_BUTTON && state == 0) {
        ButtonState = GLUT_RIGHT_BUTTON;
        X = x; Y = y;
    }
    if(button == GLUT_LEFT_BUTTON && state == 0) {
        ButtonState = GLUT_LEFT_BUTTON;
        X = x; Y = y;
    }
    if(state == 1) {
        ButtonState = -1;
    }
}
void DoMouseMoving(int x, int y) {
    if(ButtonState == GLUT_RIGHT_BUTTON) {
        azimuth     -= +(x - X) * cos(twist * M_PI / 180) + (y - Y) * sin(twist * M_PI / 180);
        elevation   -= -(x - X) * sin(twist * M_PI / 180) + (y - Y) * cos(twist * M_PI / 180);
    }
    if(ButtonState == GLUT_LEFT_BUTTON) {
        moveX += (GLfloat)(x - X) / WindowWidth  * 2;
        moveY -= (GLfloat)(y - Y) / WindowHeight * 2;
    }
    X = x; Y = y;
    glutPostRedisplay();
}


void DisplayInit() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

}

void InitLight() {
//    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    GLfloat mat_diffuse[]   = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_specular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_ambient[]   = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_shininess[]  = {15.0};
    GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]   = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_ambient[]   = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_position[]  = {-3, 5, 3.0, 0.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE , light_diffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient );
    
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE  , mat_diffuse  );
    glMaterialfv(GL_FRONT, GL_SPECULAR , mat_specular );
    glMaterialfv(GL_FRONT, GL_AMBIENT  , mat_ambient  );
    
}

void DoDisplay() {
    DisplayInit();
    
    glLoadIdentity();
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1,1,1); // X축 Red
    for(GLfloat i=-2.0; i<=2.0; i+=0.2) {
        glVertex3f(+i, -1.0, -1.0);
        glVertex3f(+i, -1.0, -50.0);
        glVertex3f(+i, +1.0, -1.0);
        glVertex3f(+i, +1.0, -50.0);
    }
    glEnd();

    PolarView(+2, elevation, azimuth, twist);
    glutSolidTeapot(0.3);
    glFlush();
}



int main(int argc, char *argv[]) {
#ifdef __APPLE__
    glutInit(&argc, argv);
#else
    glutInit(&__argc,__argv);
#endif
    
    
    glutInitWindowSize      (WindowWidth    , WindowHeight   );
    glutInitWindowPosition  (WindowPositionX, WindowPositionY);
    glutCreateWindow("KJBS2");
    
    InitLight();
    glutDisplayFunc (DoDisplay );
    glutReshapeFunc (DoReshape );
    glutKeyboardFunc(DoKeyboard);
    glutMouseFunc   (DoMouseClick);
    glutMotionFunc  (DoMouseMoving);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}

    
    
