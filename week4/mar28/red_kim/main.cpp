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

const int WindowWidth       = 600;
const int WindowHeight      = 600;
const int WindowPositionX   = 800;
const int WindowPositionY   = 000;

using namespace std;

GLfloat twist, elevation, azimuth;
GLfloat roll, pitch, yaw;
GLfloat step = 10;

void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist) {
    glTranslatef(0.0, 0.0, -radius);
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
    }
    char info[128];
    sprintf(info, "azimuth = %.1f, elevation = %.1f, twist = %.1f", azimuth, elevation, twist);
    glutSetWindowTitle(info);
    glutPostRedisplay();
}
void DoReshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
}

int X, Y, ButtonState = -1;
void DoMouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == 0) {
        ButtonState = GLUT_LEFT_BUTTON;
        X = x;
        Y = y;
        printf("MouseFunc\n");
    }
    if(button == GLUT_RIGHT_BUTTON && state == 0) {
        ButtonState = GLUT_RIGHT_BUTTON;
        X = x;
        Y = y;
    }
    if(state == 1) {
        ButtonState = -1;
    }
}
void DoMouse2(int x, int y) {
    if(ButtonState == GLUT_LEFT_BUTTON) {
        azimuth -= (x - X);
        elevation -= (y - Y);
        X = x;
        Y = y;
        printf("MotionFunc\n");
    }
    glutPostRedisplay();
//    printf("%d %d\n", x, y);
}


void DisplayInit() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
}

void InitLight() {
    GLfloat mat_diffuse[]   = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_specular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_ambient[]   = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_shininess[]  = {15.0};
    GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]   = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_ambient[]   = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_position[]  = {-3, 5, 3.0, 0.0};
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
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
    PolarView(+2, elevation, azimuth, twist);
//    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 1.0, 1.0, 0.0);
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
    glutMouseFunc   (DoMouse   );
    glutMotionFunc  (DoMouse2  );
    glutMainLoop();
    
    return EXIT_SUCCESS;
}

    
    
