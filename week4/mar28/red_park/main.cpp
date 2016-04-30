<<<<<<< HEAD
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

#define KIST_BOTH_BUTTON  0x0003
#define KIST_LEFT_BUTTON  0x0001
#define KIST_RIGHT_BUTTON 0x0002


using namespace std;
=======
#include"main.h"

GLfloat translationX,translationY,translationZ,translationStep=0.05;
GLfloat twist,elevation,azimuth,angleStep=2.0;
GLfloat scale,size,scaleStep;
GLfloat projectionMatrix[16];
int prvX,prvY,buttonState;
char file[260],info[260];

void initialize()
{
    translationX=0;
    translationY=0;
    translationZ=0;
    elevation=0;
    azimuth=0;
    twist=0;
    scale=1;
    scaleStep=0.05;
}

void DoLoad()
{
    int i;
#ifdef _DEBUG
    const char *ret="../../sample/al.obj";
#else
    const char *ret=tinyfd_openFileDialog(dialogTitle,NULL,1,filterPatterns,filterDescription,0);
#endif
    if(ret==NULL)return;
    for(i=strlen(ret)-1;i>=0;--i)if(ret[i]=='/'||ret[i]=='\\')break;
    strcpy(file,ret+i+1);
    FILE*fp=fopen(ret,"r");
    if(fp==NULL)return;
    initialize();
    aVertex.clear();
    aFace.clear();
    size=0;
    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            aVertex.push_back(v);
            if(v.getNorm()>size)size=v.getNorm();
        }
        else if(!strncmp(line,"vn ",3))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            aNormal.push_back(-v);
        }
        else if(!strncmp(line,"f ",2))
        {
            int t;
            Vector3 n;
            aFace.push_back(Face());
            char *pt=line+2;
            while(1)
            {
                sscanf(pt,"%d",&t);
                aFace.back().vertexNo.push_back(t);
                if(strstr(pt,"/"))
                {
                    pt=strstr(pt,"/")+1;
                    if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                    {
                        pt=strstr(pt,"/")+1;
                        sscanf(pt,"%d",&t);
                        n=aNormal[t-1];
                    }
                }
                aFace.back().normal.push_back(n);
                if(strstr(pt," "))pt=strstr(pt," ")+1;
                else break;
                for(i='0';i<='9';++i)
                {
                    char digit[2];
                    digit[0]=i;
                    digit[1]=0;
                    if(strstr(pt,digit))break;
                }
                if(i>'9')break;
            }
        }
    }
    fclose(fp);
}

void DoDisplay()
{
    sprintf(info,"\"%s\" - x=%.1f, y=%.1f, z=%.1f, e=%.1f, a=%.1f, t=%.1f, s=%.1f",file,translationX,translationY,translationZ,elevation,azimuth,twist,scale);
    glutSetWindowTitle(info);

    glClearColor(0.5,0.5,0.5,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
>>>>>>> 1fb0edd54321e4bb67f2a171c309f868fc167f8a

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-size,size,-size,size,-size*scale,size*scale);

    glScalef(scale,scale,scale);
    glTranslatef(translationX,translationY,translationZ);
    glRotatef(twist,0.0,0.0,1.0);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);

    GLfloat lightAmbient[]={0.5,0.5,0.5,1};
    GLfloat lightDiffuse[]={0.7,0.7,0.7,1};
    GLfloat lightSpecular[]={1,1,1,1};
    GLfloat lightPosition[]={0,0,-size,0};
    GLfloat materialAmbient[]={0.7,0.7,0.7,1};
    GLfloat materialSpecular[]={1,1,1,1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
    glMaterialf(GL_FRONT,GL_SHININESS,128);

<<<<<<< HEAD
void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist)
{
//    glTranslatef(0.0, 0.0, -radius);
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(-twist,       0.0, 0.0, 1.0);
    glRotatef(-elevation,   1.0, 0.0, 0.0);
    glRotatef(-azimuth,     0.0, 1.0, 0.0);
}
void PilotView(GLfloat roll, GLfloat pitch, GLfloat yaw)
{
    glRotatef(-roll,    0.0, 0.0, 1.0);
    glRotatef(-pitch,   1.0, 0.0, 0.0);
    glRotatef(-yaw,     0.0, 1.0, 0.0);
}
void DoKeyboard(unsigned char key, int x, int y)
{
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

void DoReshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(changeProjection)
        glOrtho(XcoordinateMin, XcoordinateMax, YcoordinateMin, YcoordinateMax, ZcoordinateMin, ZcoordinateMax);
    else
        glFrustum(-1, 1, -1, 1, 1, 15);
}

int X, Y, ButtonState = 0;
void DoMouseClick(int button, int state, int x, int y)
{
    
    if(state == GLUT_UP) {
        switch(button) {
            case GLUT_LEFT_BUTTON:
                ButtonState &= ~KIST_LEFT_BUTTON;
                break;
            case GLUT_RIGHT_BUTTON:
                ButtonState &= ~KIST_RIGHT_BUTTON;
                break;
        }
    }
    if(state == GLUT_DOWN) {
        switch(button) {
            case GLUT_LEFT_BUTTON:
                ButtonState |= KIST_LEFT_BUTTON;
                break;
            case GLUT_RIGHT_BUTTON:
                ButtonState |= KIST_RIGHT_BUTTON;
                break;
        }
        X = x;
        Y = y;
=======
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        glBegin(GL_POLYGON);
        if(it->vertexNo.size()<3)continue;
        for(unsigned int i=0;i<it->vertexNo.size();++i)
        {
            if(it->normal[i].x==0&&it->normal[i].y==0&&it->normal[i].z==0)
            {
                Vector3 ba=aVertex[it->vertexNo[1]-1]-aVertex[it->vertexNo[0]-1];
                Vector3 ca=aVertex[it->vertexNo[2]-1]-aVertex[it->vertexNo[0]-1];
                Vector3 n(ca.y*ba.z-ba.y*ca.z,ca.z*ba.x-ba.z*ca.x,ca.x*ba.y-ba.x*ca.y);
                it->normal[i]=n;
            }
            Vector3 n2(-it->normal[i].x*projectionMatrix[0]-it->normal[i].y*projectionMatrix[4]-it->normal[i].z*projectionMatrix[8],-it->normal[i].x*projectionMatrix[1]-it->normal[i].y*projectionMatrix[5]-it->normal[i].z*projectionMatrix[9],-it->normal[i].x*projectionMatrix[2]-it->normal[i].y*projectionMatrix[6]-it->normal[i].z*projectionMatrix[10]);
            n2=n2/n2.getNorm();
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
        }
        glEnd();
    }

    glFlush();
}

void DoReshape(int w,int h)
{
    // TODO
}

void DoMouse(int button,int state,int x,int y)
{
    button=glConvertButton(button);
    if(state==GLUT_DOWN)
    {
        switch(button)
        {
        case KIST_LEFT_BUTTON:
        case KIST_RIGHT_BUTTON:
            buttonState=buttonState|button;
            prvX=x;
            prvY=y;
            break;
        case KIST_SCROLL_UP:
            scale=scale*(1+scaleStep);
            break;
        case KIST_SCROLL_DOWN:
            scale=scale/(1+scaleStep);
            break;
        }
    }
    if(state==GLUT_UP)
    {
        switch(button)
        {
        case KIST_LEFT_BUTTON:
        case KIST_RIGHT_BUTTON:
            buttonState=buttonState&(~button);
            break;
        }
>>>>>>> 1fb0edd54321e4bb67f2a171c309f868fc167f8a
    }
}
<<<<<<< HEAD
void DoMouseMoving(int x, int y)
=======

void DoMouseMove(int x,int y)
>>>>>>> 1fb0edd54321e4bb67f2a171c309f868fc167f8a
{
    switch(buttonState)
    {
<<<<<<< HEAD
        case KIST_RIGHT_BUTTON:
            azimuth     -= +(x - X) * cos(twist * M_PI / 180) + (y - Y) * sin(twist * M_PI / 180);
            elevation   -= -(x - X) * sin(twist * M_PI / 180) + (y - Y) * cos(twist * M_PI / 180);
            break;
        case KIST_LEFT_BUTTON:
            moveX += (GLfloat)(x - X) / WindowWidth  * 2;
            moveY -= (GLfloat)(y - Y) / WindowHeight * 2;
            break;
        case KIST_BOTH_BUTTON:
            twist += (x - X);
            break;
=======
    case KIST_LEFT_BUTTON:
        translationX=translationX+(x-prvX)*2.0*size/scale/WindowWidth;
        translationY=translationY-(y-prvY)*2.0*size/scale/WindowHeight;
        break;
    case KIST_RIGHT_BUTTON:
        azimuth=azimuth+(x-prvX)*cos(twist*M_PI/180)-(y-prvY)*sin(twist*M_PI/180);
        elevation=elevation+(x-prvX)*sin(twist*M_PI/180)+(y-prvY)*cos(twist*M_PI/180);
        break;
    case KIST_BOTH_BUTTON:
        twist=twist+(atan2(-y+WindowHeight/2,x-WindowWidth/2)-atan2(-prvY+WindowHeight/2,prvX-WindowWidth/2))*180/M_PI;
        break;
>>>>>>> 1fb0edd54321e4bb67f2a171c309f868fc167f8a
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}

<<<<<<< HEAD

void DisplayInit()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
}

void InitLight()
{
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

void DoDisplay()
{
    DisplayInit();
    
    glLoadIdentity();
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1,1,1); // X축 Red
    for(GLfloat i=-2.0; i<=2.0; i+=0.2)
    {
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



int main(int argc, char *argv[])
{
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
=======
void DoKeyboard(unsigned char key,int x,int y)
{
    switch(key)
    {
    case 'a':
        azimuth=azimuth-angleStep;
        break;
    case 'd':
        azimuth=azimuth+angleStep;
        break;
    case 'w':
        elevation=elevation-angleStep;
        break;
    case 's':
        elevation=elevation+angleStep;
        break;
    case 'q':
        twist=twist+angleStep;
        break;
    case 'e':
        twist=twist-angleStep;
        break;
    case 'x':
        translationZ=translationZ-translationStep*size;
        break;
    case 'c':
        translationZ=translationZ+translationStep*size;
        break;
    case 'z':
        initialize();
        break;
    case ' ':
        DoLoad();
        break;
    }
    glutPostRedisplay();
}

void DoSpecial(int key,int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        translationX=translationX-translationStep*size;
        break;
    case GLUT_KEY_RIGHT:
        translationX=translationX+translationStep*size;
        break;
    case GLUT_KEY_UP:
        translationY=translationY+translationStep*size;
        break;
    case GLUT_KEY_DOWN:
        translationY=translationY-translationStep*size;
        break;
    }
    glutPostRedisplay();
}

int main(int argc,char *argv[])
{
#if !defined(_DEBUG) && !defined(__APPLE__)
    FreeConsole();
#endif

    DoLoad();

    glutInit(&argc,argv);
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutCreateWindow(NULL);

    glutDisplayFunc(DoDisplay);
    glutReshapeFunc(DoReshape);
    glutMouseFunc(DoMouse);
    glutMotionFunc(DoMouseMove);
>>>>>>> 1fb0edd54321e4bb67f2a171c309f868fc167f8a
    glutKeyboardFunc(DoKeyboard);
    glutMouseFunc   (DoMouseClick);
    glutMotionFunc  (DoMouseMoving);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}

    
    
