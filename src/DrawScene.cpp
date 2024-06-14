#include <stddef.h>
#include <gl/glut.h>
#include "DrawScene.h"
#include "Curve.h"
#include "Surface.h"
#include "Vector3D.h"

void DrawScene(int iScene)
{
	switch (iScene)
	{
	case 1:
		DrawteTrahedron();
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void DrawScene(int iScene, CCurve *pCurve)
{
	switch (iScene)
	{
	case 1:
		DrawteTrahedron();
		break;
	case 2:
		DrawCurveAndTangentLine(pCurve);
		break;
	case 3:
		DrawBezierCurve(pCurve);
		break;
	default:
		break;
	}
}

// 绘制地板和四面体
void DrawteTrahedron()
{
	// 绘制网格地板
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = 0; x < 10; x++)
	{
		float xStart = 3.0 - x * 0.6;
		for (int z = 0; z < 10; z++)
		{
			float zStart = 3.0 - z * 0.6;

			if ((z % 2) ^ (x % 2))
				glColor3f(0.1, 0.1, 0.1);
			else
				glColor3f(0.7, 0.7, 0.7);
			glVertex3f(xStart, 0, zStart);
			glVertex3f(xStart - 0.6, 0, zStart);
			glVertex3f(xStart - 0.6, 0, zStart - 0.6);
			glVertex3f(xStart, 0, zStart - 0.6);
		}
	}
	glEnd();

	// 绘制四面体，每三个点构成一个三角形面，最后两个点是之前的重复
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1, 1, 1);
	glVertex3f(0, 2, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 2.4);
	glColor3f(0, 1, 0);
	glVertex3f(-1.8, 0, -1.8);
	glColor3f(0, 0, 1);
	glVertex3f(3, 0, 0);
	glColor3f(1, 1, 1);
	glVertex3f(0, 2, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 2.4);
	glEnd();
}

void DrawCurveAndTangentLine(CCurve *m_pCurve)
{
	if (m_pCurve == NULL)
		return;

	float vcoord[3];
	int i;
	int pointnum = m_pCurve->m_pointnum;
	if (pointnum == 0)
		return;

	// 绘制曲线
	glLineWidth(2.0);
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	for (i = 0; i < pointnum - 1; i++)
	{
		vcoord[0] = m_pCurve->m_pointx[i];
		vcoord[1] = m_pCurve->m_pointy[i];
		vcoord[2] = m_pCurve->m_pointz[i];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);

		vcoord[0] = m_pCurve->m_pointx[i + 1];
		vcoord[1] = m_pCurve->m_pointy[i + 1];
		vcoord[2] = m_pCurve->m_pointz[i + 1];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
	}
	glEnd();

	// 绘制切线，这里切线长度取50
	glLineWidth(1.0);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	for (i = 0; i < pointnum - 1; i = i + 2) // 隔两个点画一个切线，不然太密集了
	{
		vcoord[0] = m_pCurve->m_pointx[i];
		vcoord[1] = m_pCurve->m_pointy[i];
		vcoord[2] = m_pCurve->m_pointz[i];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);

		// vcoord[0] = cCurve->m_pointx[i] + cCurve->m_tangentx[i] * 50.0;//取长度为50
		// vcoord[1] = cCurve->m_pointy[i] + cCurve->m_tangenty[i] * 50.0;
		// vcoord[2] = cCurve->m_pointz[i] + cCurve->m_tangentz[i] * 50.0;
		vcoord[0] = m_pCurve->m_pointx[i] + m_pCurve->m_tangentx[i] * 0.5; // 取长度为0.5
		vcoord[1] = m_pCurve->m_pointy[i] + m_pCurve->m_tangenty[i] * 0.5;
		vcoord[2] = m_pCurve->m_pointz[i] + m_pCurve->m_tangentz[i] * 0.5;
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
	}
	glEnd();
}

// 绘制Bezier曲线
void DrawBezierCurve(CCurve *m_pCurve)
{
	if (m_pCurve == NULL)
		return;

	float vcoord[3];
	int i;
	int pointnum = m_pCurve->m_pointnum;
	if (pointnum == 0)
		return;

	glLineWidth(2.0);
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	for (i = 0; i < pointnum - 1; i++)
	{
		vcoord[0] = m_pCurve->m_pointx[i];
		vcoord[1] = m_pCurve->m_pointy[i];
		vcoord[2] = m_pCurve->m_pointz[i];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);

		vcoord[0] = m_pCurve->m_pointx[i + 1];
		vcoord[1] = m_pCurve->m_pointy[i + 1];
		vcoord[2] = m_pCurve->m_pointz[i + 1];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
	}
	glEnd();
}

// 绘制Bezier曲线控制多边形
void DrawBezierControlPolygon(CCurve *m_pCurve)
{
	if (m_pCurve == NULL)
		return;

	float vcoord[3];
	int i;
	int vertexnum = m_pCurve->m_ctrlvertexnum;

	// 绘制控制顶点
	glPointSize(5.0);
	glColor3ub(255, 0, 0);
	glBegin(GL_POINTS);
	for (i = 0; i < vertexnum; i++)
	{
		vcoord[0] = m_pCurve->m_ctrlvertexx[i];
		vcoord[1] = m_pCurve->m_ctrlvertexy[i];
		vcoord[2] = m_pCurve->m_ctrlvertexz[i];
		glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
	}
	glEnd();

	// 绘制控制多边形
	if (vertexnum >= 2)
	{
		glLineWidth(1.0);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		for (i = 0; i < vertexnum - 1; i++)
		{
			vcoord[0] = m_pCurve->m_ctrlvertexx[i];
			vcoord[1] = m_pCurve->m_ctrlvertexy[i];
			vcoord[2] = m_pCurve->m_ctrlvertexz[i];
			glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
			vcoord[0] = m_pCurve->m_ctrlvertexx[i + 1];
			vcoord[1] = m_pCurve->m_ctrlvertexy[i + 1];
			vcoord[2] = m_pCurve->m_ctrlvertexz[i + 1];
			glVertex3f((GLfloat)vcoord[0], (GLfloat)vcoord[1], (GLfloat)vcoord[2]);
		}
		glEnd();
	}
}

// 绘制样条曲面
void DrawSurface(CSurface *m_pSurface)
{
	if (m_pSurface == NULL)
		return;

	GLfloat vcoord[4][3];
	int i, j, xyz;
	int pointnum[2]; //曲面在u、v方向离散点的数量
	for (int uv = 0; uv < 2; uv++)
	{
		pointnum[uv] = m_pSurface->m_pointnum[uv];
		if (pointnum[uv] == 0)
			return;
	}

	Vector3D v3d1, v3d2, v3dnormal;

	glLineWidth(2.0);
	glColor3ub(0, 0, 255);
	glBegin(GL_TRIANGLES);
	// glBegin(GL_POINTS);
	// 一个等参线间隔有两个逆时针三角形,(i,j)--(i+1,j)--(i+1,j+1)；(i,j)--(i+1,j+1)--(i,j+1)
	for (i = 0; i < pointnum[0] - 1; i++)
	{
		for (j = 0; j < pointnum[1] - 1; j++)
		{
			for (xyz = 0; xyz < 3; xyz++)
			{
				vcoord[0][xyz] = (GLfloat)m_pSurface->m_point[i][j][xyz];
				vcoord[1][xyz] = (GLfloat)m_pSurface->m_point[i + 1][j][xyz];
				vcoord[2][xyz] = (GLfloat)m_pSurface->m_point[i + 1][j + 1][xyz];
				vcoord[3][xyz] = (GLfloat)m_pSurface->m_point[i][j + 1][xyz];
			}

			// 第一个三角
			v3d1.set(vcoord[1][0] - vcoord[0][0], vcoord[1][1] - vcoord[0][1], vcoord[1][2] - vcoord[0][2]);
			v3d2.set(vcoord[2][0] - vcoord[1][0], vcoord[2][1] - vcoord[1][1], vcoord[2][2] - vcoord[1][2]);
			v3dnormal = v3d1 ^ v3d2;
			v3dnormal.SetUnit();
			glNormal3f(v3dnormal[0], v3dnormal[1], v3dnormal[2]);
			glVertex3f(vcoord[0][0], vcoord[0][1], vcoord[0][2]);
			glVertex3f(vcoord[1][0], vcoord[1][1], vcoord[1][2]);
			glVertex3f(vcoord[2][0], vcoord[2][1], vcoord[2][2]);

			// 第二个三角
			v3d1.set(vcoord[2][0] - vcoord[0][0], vcoord[2][1] - vcoord[0][1], vcoord[2][2] - vcoord[0][2]);
			v3d2.set(vcoord[3][0] - vcoord[2][0], vcoord[3][1] - vcoord[2][1], vcoord[3][2] - vcoord[2][2]);
			v3dnormal = v3d1 ^ v3d2;
			v3dnormal.SetUnit();
			glNormal3f(v3dnormal[0], v3dnormal[1], v3dnormal[2]);
			glVertex3f(vcoord[0][0], vcoord[0][1], vcoord[0][2]);
			glVertex3f(vcoord[2][0], vcoord[2][1], vcoord[2][2]);
			glVertex3f(vcoord[3][0], vcoord[3][1], vcoord[3][2]);
		}
	}
	glEnd();
}

// 绘制样条曲面控制顶点
void DrawSurfaceControlPolygon(CSurface *m_pSurface)
{
	if (m_pSurface == NULL)
		return;

	GLfloat vcoord[3], vcoordnextu[3], vcoordnextv[3];
	int i, j, k;
	int vertexnum[2];
	for (int uv = 0; uv < 2; uv++)
	{
		vertexnum[uv] = m_pSurface->m_ctrlvertexnum[uv];
		if (vertexnum[uv] <= 1)
			return;
	}

	// 绘制控制顶点
	glPointSize(5.0);
	glColor3ub(255, 0, 0);
	glBegin(GL_POINTS);
	for (i = 0; i < vertexnum[0]; i++)
	{
		for (j = 0; j < vertexnum[1]; j++)
		{
			for (k = 0; k < 3; k++)
			{
				vcoord[k] = (GLfloat)m_pSurface->m_ctrlvertex[i][j][k];
			}
			glVertex3f(vcoord[0], vcoord[1], vcoord[2]);
		}
	}
	glEnd();

	// 绘制控制多边形
	glLineWidth(1.0);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);

	for (i = 0; i < vertexnum[0] - 1; i++)
	{
		for (j = 0; j < vertexnum[1]; j++)
		{
			for (k = 0; k < 3; k++)
			{

				vcoord[k] = (GLfloat)m_pSurface->m_ctrlvertex[i][j][k];
				vcoordnextu[k] = (GLfloat)m_pSurface->m_ctrlvertex[i + 1][j][k];
			}
			// u向控制线
			glVertex3f(vcoord[0], vcoord[1], vcoord[2]);
			glVertex3f(vcoordnextu[0], vcoordnextu[1], vcoordnextu[2]);
		}
	}
	for (i = 0; i < vertexnum[0]; i++)
	{
		for (j = 0; j < vertexnum[1] - 1; j++)
		{
			for (k = 0; k < 3; k++)
			{

				vcoord[k] = (GLfloat)m_pSurface->m_ctrlvertex[i][j][k];
				vcoordnextv[k] = (GLfloat)m_pSurface->m_ctrlvertex[i][j + 1][k];
			}
			// v向控制线
			glVertex3f(vcoord[0], vcoord[1], vcoord[2]);
			glVertex3f(vcoordnextv[0], vcoordnextv[1], vcoordnextv[2]);
		}
	}
	glEnd();
}