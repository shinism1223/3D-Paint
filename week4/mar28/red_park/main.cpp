#include"main.h"

GLfloat translationX,translationY,translationZ,translationStep=0.1;
GLfloat twist,elevation,azimuth,angleStep=2.0;
GLfloat scale,scaleOriginal,scaleStep;
GLfloat projectionMatrix[16];
int prvX,prvY,buttonState;

void DoDisplay()
{
    char info[128];
    sprintf(info,"x=%.1f, y=%.1f, z=%.1f, e=%.1f, a=%.1f, t=%.1f, s=%.1f",translationX,translationY,translationZ,elevation,azimuth,twist,scale);
    glutSetWindowTitle(info);

    glClearColor(0.5,0.5,0.5,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-scale,scale,-scale,scale,-scale,scaleOriginal);

    glTranslatef(translationX,translationY,translationZ);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
    glRotatef(twist,0.0,0.0,1.0);
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
    //printf("%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n-----\n",projectionMatrix[0],projectionMatrix[4],projectionMatrix[8],projectionMatrix[12],projectionMatrix[1],projectionMatrix[5],projectionMatrix[9],projectionMatrix[13],projectionMatrix[2],projectionMatrix[6],projectionMatrix[10],projectionMatrix[14],projectionMatrix[3],projectionMatrix[7],projectionMatrix[11],projectionMatrix[15]);

    GLfloat lightAmbient[]={0.3,0.3,0.3,1};
    GLfloat lightDiffuse[]={0.7,0.7,0.7,1};
    GLfloat lightSpecular[]={1,1,1,1};
    GLfloat lightPosition[]={0,0,-scaleOriginal,0};
    GLfloat materialAmbient[]={0.7,0.7,0.7,1};
    GLfloat materialSpecular[]={1,1,1,1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
    glMaterialf(GL_FRONT,GL_SHININESS,128);

    glBegin(GL_TRIANGLES);
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        for(unsigned int i=1;i<it->vertexNo.size()-1;++i)
        {
            Vertex ba=aVertex[it->vertexNo[i]-1]-aVertex[it->vertexNo[0]-1];
            Vertex ca=aVertex[it->vertexNo[i+1]-1]-aVertex[it->vertexNo[0]-1];
            Vertex n(ca.y*ba.z-ba.y*ca.z,ca.z*ba.x-ba.z*ca.x,ca.x*ba.y-ba.x*ca.y);
            Vertex n2(-n.x*projectionMatrix[0]-n.y*projectionMatrix[4]-n.z*projectionMatrix[8],-n.x*projectionMatrix[1]-n.y*projectionMatrix[5]-n.z*projectionMatrix[9],-n.x*projectionMatrix[2]-n.y*projectionMatrix[6]-n.z*projectionMatrix[10]);
            n2=n2/n2.getLength();
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3fv(aVertex[it->vertexNo[0]-1]);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
            glVertex3fv(aVertex[it->vertexNo[i+1]-1]);
        }
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

void DoSpecial(int key,int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        translationX=translationX-translationStep;
        break;
    case GLUT_KEY_RIGHT:
        translationX=translationX+translationStep;
        break;
    case GLUT_KEY_UP:
        translationY=translationY+translationStep;
        break;
    case GLUT_KEY_DOWN:
        translationY=translationY-translationStep;
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

    scaleOriginal=scaleOriginal*sqrt(3);
    scale=scaleOriginal;
    scaleStep=scale/20;

    glutInit(&argc,argv);
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutCreateWindow("Obj File Viewer - April 3, RED_KENNY");

    glutDisplayFunc(DoDisplay);
    glutMouseFunc(DoMouse);
    glutMotionFunc(DoMouseMove);
    glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);

    glutMainLoop();

    return EXIT_SUCCESS;
}
