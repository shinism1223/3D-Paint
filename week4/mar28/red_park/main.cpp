#ifdef __APPLE__
#include <GLUT/glut.h>
#include "tinyfiledialogs.h"
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#define GLUT_BOTH_BUTTON 0x0010

using namespace std;

double scale=1,scalestep;

const int   WindowWidth       = 600;
const int   WindowHeight      = 600;
const int   WindowPositionX   = 800;
const int   WindowPositionY   = 000;

struct Vertex
{
    Vertex(float x,float y,float z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    float x,y,z;
};

struct Face
{
    vector<int> vertexNo;
};

vector<Vertex> aVertex;
vector<Face> aFace;

GLfloat xAngle, yAngle, zAngle;
GLfloat angleStep = 2.0;
float nx = 0, ny = 0, nz = 0;
bool restart = true;
float translationStep = 0.1;

float saveMatrix[16];

GLfloat twist, elevation, azimuth;
GLfloat roll, pitch, yaw;
GLfloat step = 10;

GLfloat moveX, moveY, moveZ=-2;
GLfloat stepMove = 0.5;

#ifdef __APPLE__
FILE*LoadFt(){
    while(1) {
        char const *openFileName = tinyfd_openFileDialog (
                                                         "choose OBJ file",
                                                         "",
                                                         0,
                                                         NULL,
                                                         NULL,
                                                         0);
        if(openFileName == NULL) break;
        int nameLength = (int)strlen(openFileName);
        if(openFileName[nameLength-3] == 'o' && openFileName[nameLength-2] == 'b' && openFileName[nameLength-1] == 'j')
            return fopen(openFileName, "r");
        tinyfd_messageBox("FileTypeError",
                          "Please choose obj file",
                          "ok", "error", 0);
    }
    return NULL;
}
#else
FILE*LoadFt(const char*Title,const char*Filter,const char*Extension="",const char*Initial="")
{
    char file_path[1024],file_name[1024];
    strcpy(file_path,Initial);
    OPENFILENAME open_file={sizeof(OPENFILENAME),GetForegroundWindow(),0,Filter,NULL,0,1,file_path,sizeof(file_path),file_name,sizeof(file_name),NULL,Title,OFN_ENABLESIZING|OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,0,12,NULL,0,NULL,NULL};
    if(!GetOpenFileName(&open_file))return NULL;
    FILE*fp=fopen(file_path,"rt");
    return fp;
} // 사용 예 : FILE*fl=LoadFt("KENNYC Load","KENNYC TEST(*.kct)\0*.kct\0KENNYC TEST2(*.kc2)\0*.kc2\0\0"); - 제목 : KENNYC Load, 파일 형식 : KENNYC TEST(*.kct)와 KENNYC TEST2(*.kc2)
#endif

inline void glVertex3f(Vertex &v)
{
    glVertex3f(v.x,v.y,v.z);
}

int X, Y, ButtonState = -1;
void DoMouse(int button, int state, int x, int y)
{
    if(state==GLUT_UP)
    {
        ButtonState = -1;
        return;
    }
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(ButtonState==GLUT_RIGHT_BUTTON)ButtonState=GLUT_BOTH_BUTTON;
        else ButtonState = GLUT_LEFT_BUTTON;
        X = x;
        Y = y;
        break;
    case GLUT_RIGHT_BUTTON:
        if(ButtonState==GLUT_LEFT_BUTTON)ButtonState=GLUT_BOTH_BUTTON;
        else ButtonState = GLUT_RIGHT_BUTTON;
        X = x;
        Y = y;
        break;
    case 3:
        if(scale>scalestep)scale=scale-scalestep; // TODO: Zoom at that point
        break;
    case 4:
        scale=scale+scalestep;
        break;
    }
    glutPostRedisplay();
}

void DoMouseMove(int x, int y)
{
    switch(ButtonState)
    {
    case GLUT_LEFT_BUTTON:
        moveX += (GLfloat)(x - X) / WindowWidth  * scale * 2;
        moveY -= (GLfloat)(y - Y) / WindowHeight * scale * 2;
        break;
    case GLUT_RIGHT_BUTTON:
        azimuth -= (x - X);
        elevation -= (y - Y);
        break;
    case GLUT_BOTH_BUTTON:
        twist += (x - X);
    }
    X = x; Y = y;
    glutPostRedisplay();
}

void DisplayInit() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 블랜딩이 켜져 있어야 알리아싱이 제대로 된다.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // 고품질 출력을 위한 힌트
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_FLAT);
}

void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist) {
//    glTranslatef(0.0, 0.0, -radius);
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(-elevation,   1.0, 0.0, 0.0);
    glRotatef(-azimuth,     0.0, 1.0, 0.0);
    glRotatef(-twist,       0.0, 0.0, 1.0);
}

void DoDisplay()
{
    DisplayInit();
/*
    if(restart == true) {
        glLoadIdentity(); //원상태로 복구
        glOrtho(-scale, scale, -scale, scale, -scale, scale); // 카메라의 보는 시각 증가
        //기본값은 다 1 입니다.
        //기존의 모델이 준 것이라고 생각하면 됩니다
        //opengl에 저장되어 있는 매트릭스 가져오는 것입니다
        glGetFloatv(GL_MODELVIEW_MATRIX, saveMatrix);
        restart = false;
    }else{
        glLoadMatrixf(saveMatrix);
    }*/
    glLoadIdentity(); //원상태로 복구
    glOrtho(-scale, scale, -scale, scale, -scale, scale); // 카메라의 보는 시각 증가

    PolarView(+2, elevation, azimuth, twist);

    glTranslatef(nx, ny, nz); //평행이동
    glRotatef(xAngle, 1.0f, 0.0f, 0.0f); //축회전
    glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

    glGetFloatv(GL_MODELVIEW_MATRIX, saveMatrix);

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

    glColor3f(1,1,1);
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        glVertex3f(aVertex[it->vertexNo[0]-1]);
        for(unsigned int i=1;i<it->vertexNo.size();++i)
        {
            glVertex3f(aVertex[it->vertexNo[i]-1]);
            glVertex3f(aVertex[it->vertexNo[i]-1]);
        }
        glVertex3f(aVertex[it->vertexNo[0]-1]);
    }
    glEnd();

    glFlush();
}

void DoKeyboard(unsigned char key, int x, int y)
{
    bool isNzChange = false;
    //xAngle = yAngle = zAngle = 0;
    //nx = ny = nz = 0; restart = false;
    switch(key) {
        case 'a':yAngle += angleStep;break;
        case 'd':yAngle -= angleStep;break;
        case 'w':xAngle += angleStep;break;
        case 's':xAngle -= angleStep;break;
        case 'q':zAngle += angleStep;break;
        case 'e':zAngle -= angleStep;break;
        case 'z':restart = true; break;
        case 'n':nz -= translationStep; isNzChange = true; break;
        case 'm':nz += translationStep; isNzChange = true; break;
    }
    char info[128];
    if(isNzChange == false) {
        sprintf(info, "x=%.1f, y=%.1f, z=%.1f", xAngle, yAngle, zAngle);
        glutSetWindowTitle(info);
    }else{
        sprintf(info, "x=%.2f, y=%.2f z=%2.f", nx, ny, nz);
        glutSetWindowTitle(info);
    }
    glutPostRedisplay();
}

void DoSpecial(int key, int x, int y)
{
    //xAngle = yAngle = zAngle = 0;
    //nx = ny = nz = 0; restart = false;
    switch(key) {
        case GLUT_KEY_LEFT:
            nx -= translationStep;
            break;
        case GLUT_KEY_RIGHT:
            nx += translationStep;
            break;
        case GLUT_KEY_UP:
            ny += translationStep;
            break;
        case GLUT_KEY_DOWN:
            ny -= translationStep;
            break;
    }
    char info[128];
    sprintf(info, "x=%.2f, y=%.2f z=%2.f", nx, ny, nz);
    glutSetWindowTitle(info);
    glutPostRedisplay();
}

#ifdef __APPLE__
int main(int argc, char *argv[])
{
    FILE *fp=LoadFt();
#else
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
    FILE *fp=LoadFt("Object File Load","Object File(*.obj)\0*.obj\0\0");
#endif
    if(!fp)return 0;

    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            float x,y,z;
            sscanf(line+2,"%f%f%f",&x,&y,&z);
            if(abs(x)>scale)scale=abs(x);
            if(abs(y)>scale)scale=abs(y);
            if(abs(z)>scale)scale=abs(z);
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
    scalestep=scale/20;
    fclose(fp);

#ifdef __APPLE__
    glutInit(&argc, argv);
#else
    glutInit(&__argc,__argv);
#endif
    glutInitWindowSize      (WindowWidth    , WindowHeight   );
    glutInitWindowPosition  (WindowPositionX, WindowPositionY);
    glutCreateWindow("Obj File Viewer - Mar 30, RED_KENNY");

    glutDisplayFunc(DoDisplay);
    glutMouseFunc(DoMouse);
    glutMotionFunc(DoMouseMove);
    glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);

    glutMainLoop();

    return EXIT_SUCCESS;
}
