#include<stdio.h>
#include<windows.h>
#include "resource/resource.h"
#include "mainwindow.h"
#include "childwindow.h"
#include "mesh.h"
#include "parser.h"
#include "glcontext.h"

extern MainWindow window_main; // window_main�� �ٸ� cpp������ �����ֵ���
extern HINSTANCE g_hInst;

unsigned WINAPI Process(void *arg);
