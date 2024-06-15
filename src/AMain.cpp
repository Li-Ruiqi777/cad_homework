// AMain.cpp : 定义控制台应用程序的入口点。
//

#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <gl/gl.h>
#include <gl/glut.h>
#include "camera.h"
#include "DrawScene.h"
#include "curve.h"
#include "surface.h"
using namespace std;

// Light values and coordinates
GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};

// 负责各种典型视图与旋转平移
CCamera cCamera;
// 创建曲线
CCurve *cCurve = nullptr;
// 菜单项
int Menu = 0;
// 创建曲面
CSurface *cSurface = nullptr;

// 改变视窗大小，宽度和高度
void ChangeSize(int w, int h)
{
	if (w <= 0 || h <= 0)
	{
		return;
	}
	cCamera.SetScreen(w, h); // 设置viewport---//glViewport (0,0,w,h);
	cCamera.ProjectionAndLookatForDisplay();

	glutPostRedisplay();
}

// 通过键盘的上、下、左、右、上页、下页按键进行视图旋转和缩放
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		cCamera.RotateView(0.0, -0.1);

	if (key == GLUT_KEY_DOWN)
		cCamera.RotateView(0.0, 0.1);

	if (key == GLUT_KEY_LEFT)
		cCamera.RotateView(0.1, 0.0);

	if (key == GLUT_KEY_RIGHT)

		cCamera.RotateView(-0.1, 0.0);
	if (key == GLUT_KEY_PAGE_UP)
		cCamera.Zoom(0.92);

	if (key == GLUT_KEY_PAGE_DOWN)
		cCamera.Zoom(1.08);

	// Refresh the Window
	glutPostRedisplay();
}

// Called to draw scene
void Display(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cCamera.ProjectionAndLookatForDisplay();

	// Draw our scene here
	if (Menu == 1)
	{
		// case 1 tetrohedron
		DrawScene(1);
	}
	else if (Menu == 2)
	{
		// case 2 Polynomial Curve
		DrawScene(2, cCurve);
	}
	else if (Menu == 3 || Menu == 4 || Menu == 5 || Menu == 6 || Menu==10)
	{
		// case 3 or 4 or 5 Bezier Curve
		DrawScene(3, cCurve);
		DrawBezierControlPolygon(cCurve);
	}
	else if (Menu == 7 || Menu == 8)
	{
		DrawScene(3, cCurve);
		DrawBezierControlPolygon(cCurve);
	}
	else if (Menu == 9)
	{
		DrawSurface(cSurface);
		DrawSurfaceControlPolygon(cSurface);
	}
	// Restore transformations
	glPopMatrix();
	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering context.
void InitRC()
{
	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable lighting
	////////////////////////
	glEnable(GL_LIGHTING);
	/////////////////////////

	// Setup and enable light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);

	glDisable(GL_BLEND); // 不透明
	glDisable(GL_TEXTURE_2D);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 10);
}

// Reset flags as appropriate in response to menu selections
void ProcessMenu(int value)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	switch (value)
	{
	case 1:
		cCamera.SetViewType(VIEW_ISOMETRIC);
		cCamera.ZoomAll(-1.8, 0, -1.8, 3, 2, 2.4);
		Menu = 1;
		break;
	case 2:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCurve->CubicPolynomialCurve();
		cCamera.SetViewType(VIEW_ISOMETRIC);
		cCamera.ZoomAll(-1, -1, -1, 1, 1, 1);
		Menu = 2;
		break;
	case 3:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCurve->QuadraticBezierCurve();
		cCamera.SetViewType(VIEW_TOP);
		cCamera.ZoomAll(-100, -100, -100, 100, 100, 100);
		Menu = 3;
		break;
	case 4:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCurve->m_clicknum = 0;
		cCurve->QuadraticBezierCurveByClick();
		Menu = 4;
		break;
	case 5:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCamera.SetViewType(VIEW_TOP);
		// cCurve->ReadBezierCurveControlVertices("BezierCurve1Leaf.txt");
		// cCurve->ReadBezierCurveControlVertices("BezierCurve2Vase.txt");
		cCurve->ReadBezierCurveControlVertices("BezierCurve3Flower.txt");
		// cCurve->ReadBezierCurveControlVertices("BezierCurve4Clover.txt");
		// cCurve->QuadraticBezierCurveNPieces();
		cCurve->DecasteljauBezierCurve();

		Menu = 5;
		break;
	case 6:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCamera.SetViewType(VIEW_TOP);
		cCurve->InterpolateArcByBezierCurve(3);
		cCurve->DecasteljauBezierCurve();
		Menu = 6;
		break;
	case 7:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCurve->m_clicknum = 0;
		Menu = 7;
		break;
	case 8:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCurve->ReadBsplineCurveControlVertices("BSplineCurve1Vase.txt");
		cCurve->CoxdeBoorBSplineCurve();
		Menu = 8;
		break;
	case 9:
		if (cSurface == NULL)
		{
			cSurface = new CSurface;
		}
		cCamera.Init();
		cCamera.SetScreen(w, h);
		cCamera.SetViewRect(w, h);
		cCamera.SetViewType(VIEW_FRONT);
		cSurface->BiQuadraticBezierSurface();
		Menu = 9;
		break;
	case 10:
		if (cCurve == NULL)
		{
			cCurve = new CCurve;
		}
		cCurve->CubicBezierCurve();
		cCamera.SetViewType(VIEW_TOP);
		cCamera.ZoomAll(-200, -200, -200, 200, 200, 200);
		Menu = 10;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

// 鼠标响应函数
void ProcessMouse(int button, int action, int x, int y)
{
	double width, height;

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		if (Menu == 4) // 点击屏幕绘制Bezier曲线
		{
			if (cCurve == NULL)
				return;
			if (cCurve->m_clicknum < 200)
			{
				// 由于鼠标的窗口坐标是以左上角为原点，而我们的viewport是以屏幕中心为原点，且y轴与鼠标的窗口坐标相反
				// 因此需要进行换算
				cCamera.GetViewRect(width, height);
				cCurve->m_ctrlvertexx[cCurve->m_clicknum] = (float)x - width / 2.0;
				cCurve->m_ctrlvertexy[cCurve->m_clicknum] = -((float)y - height / 2.0);
				cCurve->m_ctrlvertexz[cCurve->m_clicknum] = 0.0;
				cCurve->m_clicknum++;

				// 给定控制顶点来计算Bezier曲线
				if (cCurve->m_clicknum == 3)
					cCurve->QuadraticBezierCurveByClick();
			}
		}
		if (Menu == 7) // 点击屏幕绘制三次BSPline曲线
		{
			if (cCurve == NULL)
				return;
			if (cCurve->m_clicknum < 200)
			{
				cCamera.GetViewRect(width, height);
				cCurve->m_ctrlvertexx[cCurve->m_clicknum] = (float)x - width / 2.0;
				cCurve->m_ctrlvertexy[cCurve->m_clicknum] = -((float)y - height / 2.0);
				cCurve->m_ctrlvertexz[cCurve->m_clicknum] = 0.0;
				cCurve->m_clicknum++;
				cCurve->m_ctrlvertexnum = cCurve->m_clicknum;

				// 给定控制顶点来计算BSpline曲线
				if (cCurve->m_clicknum >= 4)
					cCurve->CubicBsplineCurveByClick();
			}
		}
	}
	glutPostRedisplay();
}

int main()
{
	// 创建绘图窗口
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("CAX");

	// 创建菜单
	glutCreateMenu(&ProcessMenu);
	glutAddMenuEntry("绘制四面体", 1);
	glutAddMenuEntry("绘制三次多项式曲线", 2);
	glutAddMenuEntry("绘制二次Bezier曲线", 3);
	glutAddMenuEntry("鼠标点击绘制二次Bezier曲线", 4);
	glutAddMenuEntry("读取文件绘制Bezier曲线", 5);
	glutAddMenuEntry("插值1/4圆弧的Bezier曲线", 6);
	glutAddMenuEntry("鼠标点击绘制三次BSpline曲线", 7);
	glutAddMenuEntry("读取文件绘制n次BSpline曲线", 8);
	glutAddMenuEntry("绘制双二次Bezier曲面", 9);
	glutAddMenuEntry("绘制3次Bezier曲线", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// 相机初始化位置设置
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	cCamera.Init();
	cCamera.SetScreen(w, h);
	cCamera.SetViewRect(w, h);
	cCamera.ProjectionAndLookatForDisplay();

	// glut回调函数
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	InitRC();
	glutDisplayFunc(Display);
	glutMouseFunc(ProcessMouse);
	glutMainLoop();

	return 0;
}
