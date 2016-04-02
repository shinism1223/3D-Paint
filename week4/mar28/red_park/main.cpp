#include"main.h"

GLfloat translationX,translationY,translationZ,translationStep=0.1;
GLfloat twist,elevation,azimuth,angleStep=2.0;
GLfloat scale,scaleOriginal,scaleStep;
int prvX,prvY,buttonState;

void DoDisplay()
{
    char info[128];
    sprintf(info,"tX=%.1f, tY=%.1f, tZ=%.1f, e=%.1f, a=%.1f, t=%.1f, s=%.1f",translationX,translationY,translationZ,elevation,azimuth,twist,scale);
    glutSetWindowTitle(info);

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glLoadIdentity();
    glOrtho(-scale, scale, -scale, scale, -scale, scale);

    glTranslatef(translationX,translationY,translationZ);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
    glRotatef(twist,0.0,0.0,1.0);

#ifdef _DEBUG
    glPointSize(20.0);
    glBegin(GL_POINTS);
    glColor3f(1,1,0); // 원점 Yellow
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);

    glColor3f(1,0,0); // X축 Red
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(2.0, 0.0, 0.0);
    glColor3f(0,1,0); // Y축 Green
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 2.0, 0.0);
    glColor3f(0,0,1); // Z축 Blue
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 2.0);
#endif // _DEBUG

    glLineWidth(3);
    glBegin(GL_LINES);

    glColor3f(1,1,1);
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        glVertex3fv(aVertex[it->vertexNo[0]-1]);
        for(unsigned int i=1;i<it->vertexNo.size();++i)
        {
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
        }
        glVertex3fv(aVertex[it->vertexNo[0]-1]);
    }
    glEnd();

    glFlush();
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
            if(scale>scaleStep)scale=scale-scaleStep; // TODO: Zoom at that point
            break;
        case KIST_SCROLL_DOWN:
            scale=scale+scaleStep;
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
    }
    glutPostRedisplay();
}

void DoMouseMove(int x,int y)
{
    switch(buttonState)
    {
    case KIST_LEFT_BUTTON:
        translationX=translationX+(x-prvX)*2.0*scale/WindowWidth;
        translationY=translationY-(y-prvY)*2.0*scale/WindowHeight;
        break;
    case KIST_RIGHT_BUTTON:
        azimuth=azimuth+(x-prvX)*cos(twist*M_PI/180)+(y-prvY)*sin(twist*M_PI/180);
        elevation=elevation-(x-prvX)*sin(twist*M_PI/180)+(y-prvY)*cos(twist*M_PI/180);
        break;
    case KIST_BOTH_BUTTON:
        twist=twist+(atan2(-y+WindowHeight/2,x-WindowWidth/2)-atan2(-prvY+WindowHeight/2,prvX-WindowWidth/2))*180/M_PI;
        break;
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}

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
        translationZ=translationZ-translationStep;
        break;
    case 'c':
        translationZ=translationZ+translationStep;
        break;
    case 'z':
        translationX=0;
        translationY=0;
        translationZ=0;
        elevation=0;
        azimuth=0;
        twist=0;
        scale=scaleOriginal;
        break;
    }
    glutPostRedisplay();
}

void DoSpecial(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        translationX -= translationStep;
        break;
    case GLUT_KEY_RIGHT:
        translationX += translationStep;
        break;
    case GLUT_KEY_UP:
        translationY += translationStep;
        break;
    case GLUT_KEY_DOWN:
        translationY -= translationStep;
        break;
    }
    glutPostRedisplay();
}

int main(int argc,char *argv[])
{
    FILE *fp;
    const char *filterPatterns[]={"*.obj"};
    if((fp=fopen(tinyfd_openFileDialog("Load Obj File","",1,filterPatterns,"Object files",0),"r"))==NULL)return 0;
    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            float x,y,z;
            sscanf(line+2,"%f%f%f",&x,&y,&z);
            if(fabs(x)>scaleOriginal)scaleOriginal=fabs(x);
            if(fabs(y)>scaleOriginal)scaleOriginal=fabs(y);
            if(fabs(z)>scaleOriginal)scaleOriginal=fabs(z);
            aVertex.push_back(Vertex(x,y,z));
        }
        else if(!strncmp(line,"f ",2))
        {
            int t;
            aFace.push_back(Face());
            int pt=2;
            while(1)
            {
                sscanf(line+pt,"%d",&t);
                aFace.back().vertexNo.push_back(t);
                if(!strstr(line+pt," "))break;
                else pt=(int)(strstr(line+pt," ")-line)+1;
            }
        }
    }
    fclose(fp);

    scale=scaleOriginal;
    scaleStep=scale/20;

    glutInit(&argc,argv);
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutInitWindowPosition(WindowPositionX,WindowPositionY);
    glutCreateWindow("Obj File Viewer - April 2, RED_KENNY");

    glutDisplayFunc(DoDisplay);
    glutMouseFunc(DoMouse);
    glutMotionFunc(DoMouseMove);
    glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);

    glutMainLoop();

    return EXIT_SUCCESS;
}
