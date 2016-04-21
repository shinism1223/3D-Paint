#include"main.h"

GLfloat translationX,translationY,translationZ,translationStep=0.05;
GLfloat twist,elevation,azimuth,angleStep=2.0;
GLfloat scale,size,scaleStep;
GLfloat projectionMatrix[16];
int prvX,prvY,buttonState;
char file[260],info[260];

map <int, bool> isPress;
int mode = -1;

bool debug = true;
bool showGridline = true;
int plus = 0;

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
    
    Camera = CAMERA();
    
    prvX = -1; prvY = -1;

    mode = KIST_CAMERA_MODE;
}

void DoLoad()
{
    int i;
    const char *ret=tinyfd_openFileDialog(dialogTitle,NULL,1,filterPatterns,filterDescription,0);
    if(ret==NULL)return;
    for(i=(int)strlen(ret)-1;i>=0;--i)if(ret[i]=='/'||ret[i]=='\\')break;
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

void DoIdle() {
    if(isPress['a'] || isPress['A'])
        Camera.position = Camera.position - Camera.xAxis *(float)0.05;
    if(isPress['d'] || isPress['D'])
        Camera.position = Camera.position + Camera.xAxis *(float)0.05;
    if(isPress['w'] || isPress['W'])
        Camera.position = Camera.position + Camera.normal*(float)0.05;
    if(isPress['s'] || isPress['S'])
        Camera.position = Camera.position - Camera.normal*(float)0.05;
    glutPostRedisplay();
}

void DoDisplayInit()
{
    sprintf(info,"\"%s\" - x=%.1f, y=%.1f, z=%.1f, e=%.1f, a=%.1f, t=%.1f, s=%.1f",file,translationX,translationY,translationZ,elevation,azimuth,twist,scale);
    glutSetWindowTitle(info);
    
    glClearColor(0,0,0,1); // Background Color

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
}
void DoDisplayMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-size,size,-size,size,-size*scale,size*scale);
    glFrustum(-1, 1, -1, 1, 1, 50);
    gluLookAt(Camera.position, Camera.position + Camera.normal, Camera.yAxis);
    glScalef(scale,scale,scale);
    glTranslatef(translationX,translationY,translationZ);
    glRotatef(twist,0.0,0.0,1.0);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
    
}
void DoDisplayLightOn()
{
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
}
void DoDisplayGridline()
{
    if(!showGridline) return;
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1,1,1); // X축 Red
    for(GLfloat i=-2.0; i<=2.0; i+=0.8) {
        for(GLfloat j=-2.0; j<=2.0; j+=0.8) {
            glVertex3f(i, j, +50.0);
            glVertex3f(i, j, -50.0);
            glVertex3f(i, j, +50.0);
            glVertex3f(i, j, -50.0);
        }
    }
    glEnd();
}
void DoDisplaySample()
{
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
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
            Vector3 n2(-it->normal[i].x*projectionMatrix[0]-it->normal[i].y*projectionMatrix[4]-it->normal[i].z*projectionMatrix[8]
                       ,-it->normal[i].x*projectionMatrix[1]-it->normal[i].y*projectionMatrix[5]-it->normal[i].z*projectionMatrix[9]
                       ,-it->normal[i].x*projectionMatrix[2]-it->normal[i].y*projectionMatrix[6]-it->normal[i].z*projectionMatrix[10]);
            n2=n2/n2.getNorm();
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
        }
        glEnd();
    }
}
void DoDisplayString()
{
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawBitmapText((char *)"z, gird(l)ine, (m)ode", -1, -0.98, Camera.position.y-1);
}
void DoDisplay()
{
    DoDisplayInit();
    DoDisplayMatrix();
    DoDisplayLightOn();
    DoDisplayGridline();
    DoDisplaySample();
    DoDisplayString();
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
    }
    glutPostRedisplay();
}

void DoMouseMove(int x,int y)
{
    switch(buttonState)
    {
    case KIST_LEFT_BUTTON:
        translationX=translationX+(x-prvX)*2.0*size/scale/WindowWidth;
        translationY=translationY-(y-prvY)*2.0*size/scale/WindowHeight;
        break;
    case KIST_RIGHT_BUTTON:
        Camera.azimuth  =Camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(Camera.twist*M_PI/180)
                                         -1.0*(y-prvY)/WindowHeight*M_PI*sin(Camera.twist*M_PI/180);
        Camera.elevation=Camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(Camera.twist*M_PI/180)
                                         +1.0*(y-prvY)/WindowHeight*M_PI*cos(Camera.twist*M_PI/180);
        
        Camera.normal = rotateVector(Camera.baseNormal, Camera.baseYAxis, -Camera.azimuth);
        Camera.xAxis  = rotateVector(Camera.baseXAxis , Camera.baseYAxis, -Camera.azimuth);
        Camera.normal = rotateVector(Camera.normal    , Camera.xAxis    , -Camera.elevation);
        Camera.yAxis  = rotateVector(Camera.baseYAxis , Camera.xAxis    , -Camera.elevation);
        break;
    case KIST_BOTH_BUTTON:
        twist=twist+(atan2(-y+WindowHeight/2,x-WindowWidth/2)-atan2(-prvY+WindowHeight/2,prvX-WindowWidth/2))*180/M_PI;
        break;
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}
void DoMousePassiveMove(int x, int y)
{
    if(mode == KIST_SELECT_MODE) {
        prvX=x;
        prvY=y;
        return;
    }
    if(x < 10 || y < 10 || x > WindowWidth-10 || y > WindowHeight-10)
    {
        glutWarpPointer(WindowWidth/2, WindowHeight/2);
        prvX = WindowWidth / 2;
        prvY = WindowHeight / 2;
    }
    else
    {
        Camera.azimuth  =Camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(Camera.twist*M_PI/180)
        -1.0*(y-prvY)/WindowHeight*M_PI*sin(Camera.twist*M_PI/180);
        Camera.elevation=Camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(Camera.twist*M_PI/180)
        +1.0*(y-prvY)/WindowHeight*M_PI*cos(Camera.twist*M_PI/180);
        
        Camera.normal = rotateVector(Camera.baseNormal, Camera.baseYAxis, -Camera.azimuth);
        Camera.xAxis  = rotateVector(Camera.baseXAxis , Camera.baseYAxis, -Camera.azimuth);
        Camera.normal = rotateVector(Camera.normal    , Camera.xAxis    , -Camera.elevation);
        Camera.yAxis  = rotateVector(Camera.baseYAxis , Camera.xAxis    , -Camera.elevation);
        prvX=x;
        prvY=y;
    }
    
    glutPostRedisplay();
}

void DoKeyboardUp(unsigned char key, int x, int y) {
    isPress[(int)key] = false;
}

void DoKeyboard(unsigned char key,int x,int y)
{
    isPress[(int)key] = true;
    switch(key)
    {
    /*
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
    */
    case 'x':
        translationZ=translationZ-translationStep*size;
        break;
    case 'c':
        translationZ=translationZ+translationStep*size;
        break;
    case 'z':
        initialize();
        break;
    case 'g':
        debug = !debug;
        break;
    case 'l':
        showGridline = !showGridline;
        break;
    case 'p':
        Camera.position.z += 1;
        break;
    case ' ':
        DoLoad();
        break;
    case 'm':
        if(mode == KIST_SELECT_MODE)
        {
            mode = KIST_CAMERA_MODE;
            glutSetCursor(GLUT_CURSOR_NONE); // 커서를 가린다.
        }else if(mode == KIST_CAMERA_MODE)
        {
            mode = KIST_SELECT_MODE;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
        break;
    }
    glutPostRedisplay();
}

void DoSpecial(int key,int x,int y)
{
//    isPress[key + 256] = true;
    printf("SPECIAL %d\n", key);
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
    glutPassiveMotionFunc(DoMousePassiveMove);
    glutKeyboardFunc(DoKeyboard);
    glutKeyboardUpFunc(DoKeyboardUp);
    glutSpecialFunc(DoSpecial);
    glutIdleFunc(DoIdle);
#ifdef __APPLE__
    CGSetLocalEventsSuppressionInterval(0.0);
    // OS X 에서 기본 설정이 0.25라서 끊기는 현상이 발생하는 것을 방지.
    // http://goo.gl/7Zxkmp
#endif
    glutSetCursor(GLUT_CURSOR_NONE); // 커서를 가린다.
    
    glutMainLoop();

    return EXIT_SUCCESS;
}
