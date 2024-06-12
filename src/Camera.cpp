#include "camera.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

CCamera::CCamera(void)
{
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{
	m_eye = Vector3D(0, 0, 1000);
	m_ref = Vector3D(0, 0, 0);

	m_far = 10000;
	m_near = 1;

	m_width = 1920.0;
	m_height = 1080.0;

	m_vec_up = Vector3D(0, 1, 0);

	m_screen[0] = 1920;
	m_screen[1] = 1080;
}

void CCamera::ProjectionAndLookatForDisplay()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_RENDER);

	double left = -m_width / 2.0;
	double right = m_width / 2.0;
	double bottom = -m_height / 2.0;
	double top = m_height / 2.0;

	glOrtho(left, right, bottom, top, m_near, m_far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_ref.x, m_ref.y, m_ref.z, m_vec_up.x, m_vec_up.y, m_vec_up.z);
}

void CCamera::Selection(int xPos, int yPos)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_SELECT);
	gluPickMatrix(xPos, vp[3] - yPos, 1, 1, vp);

	double left = -m_width / 2.0;
	double right = m_width / 2.0;
	double bottom = -m_height / 2.0;
	double top = m_height / 2.0;

	glOrtho(left, right, bottom, top, m_near, m_far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_ref.x, m_ref.y, m_ref.z, m_vec_up.x, m_vec_up.y, m_vec_up.z);
}

void CCamera::SetScreen(int x, int y)
{
	glViewport(0, 0, x, y);
	if (y == 0)
		y = 1;
	double ratio = (double)x / (double)y;

	m_width *= (double)x / m_screen[0];
	m_height *= (double)y / m_screen[1];
	m_width = m_height * ratio;

	m_screen[0] = x;
	m_screen[1] = y;
}

void CCamera::SetEye(double eye_x, double eye_y, double eye_z)
{
	m_eye.x = eye_x;
	m_eye.y = eye_y;
	m_eye.z = eye_z;
}

void CCamera::SetRef(double ref_x, double ref_y, double ref_z)
{
	m_ref.x = ref_x;
	m_ref.y = ref_y;
	m_ref.z = ref_z;
}

void CCamera::SetVecUp(double up_dx, double up_dy, double up_dz)
{
	m_vec_up.x = up_dx;
	m_vec_up.y = up_dy;
	m_vec_up.z = up_dz;
}

void CCamera::SetViewRect(double width, double height)
{
	m_width = width;
	m_height = height;
	double aspect = m_screen[0] / m_screen[1];
	m_width = m_height * aspect;
}

void CCamera::GetViewRect(double &width, double &height)
{
	width = m_width;
	height = m_height;
}

void CCamera::Zoom(double scale)
{
	if (scale > 0.0)
	{
		m_width *= scale;
		m_height *= scale;
	}
}

void CCamera::ZoomAll(double x0, double y0, double z0, double x1, double y1, double z1)
{
	double width, height;
	double xl, yl, zl;
	xl = x1 - x0;
	yl = y1 - y0;
	zl = z1 - z0;

	width = max(max(xl, yl), zl);
	height = max(max(xl, yl), zl);

	SetViewRect(width / 0.8, height / 0.8);

	Vector3D vec = m_eye - m_ref;
	m_ref.x = float(x0 + x1) / 2;
	m_ref.y = float(y0 + y1) / 2;
	m_ref.z = float(z0 + z1) / 2;
	m_eye = m_ref + vec;
}

void CCamera::SetViewType(ViewType type)
{
	double r;
	Vector3D vec;

	vec = m_ref - m_eye;
	r = vec.GetLen();

	if (r < 0.001)
		r = 50.0;
	if (r > 10000)
		r = 10000;

	switch (type)
	{
	case VIEW_FRONT:
		m_eye = m_ref + Vector3D(0, r, 0);
		m_vec_up = Vector3D(0, 0, 1);
		break;
	case VIEW_BACK:
		m_eye = m_ref + Vector3D(0, -r, 0);
		m_vec_up = Vector3D(0, 0, 1);
		break;
	case VIEW_TOP:
		m_eye = m_ref + Vector3D(0, 0, r);
		m_vec_up = Vector3D(0, 1, 0);
		break;
	case VIEW_BOTTOM:
		m_eye = m_ref + Vector3D(0, 0, -r);
		m_vec_up = Vector3D(0, 1, 0);
		break;
	case VIEW_LEFT:
		m_eye = m_ref + Vector3D(r, 0, 0);
		m_vec_up = Vector3D(0, 0, 1);
		break;
	case VIEW_RIGHT:
		m_eye = m_ref + Vector3D(-r, 0, 0);
		m_vec_up = Vector3D(0, 0, 1);
		break;
	case VIEW_ISOMETRIC:
		m_eye = m_ref + Vector3D(1, 1, 1).GetNormal() * r;
		UpdateUpVec();
		break;
	default:
		break;
	}
}

void CCamera::MoveView(double dx, double dy)
{
	// 鼠标在视口中移动距离占视口尺寸比例
	// 移眼睛距离占视景体比例同上比例
	double dpx = dx / m_screen[0];
	double dpy = dy / m_screen[1];

	Vector3D vec = m_ref - m_eye;
	vec.SetUnit();

	Vector3D xUp, yUp;
	xUp = vec ^ m_vec_up;
	yUp = xUp ^ vec;

	m_eye -= xUp * m_width * dpx + yUp * m_height * dpy;
	m_ref -= xUp * m_width * dpx + yUp * m_height * dpy;
}

void CCamera::RotateView(double a, double b)
{
	Vector3D vecfront = m_eye - m_ref;
	double distance = vecfront.GetLen();
	vecfront.SetUnit();

	Vector3D w = m_vec_up ^ vecfront;
	w.SetUnit();
	vecfront = vecfront * cos(a) + w * sin(a);
	w = m_vec_up ^ vecfront;
	w.SetUnit();
	vecfront = vecfront * cos(b) + m_vec_up * sin(b);
	m_vec_up = vecfront ^ w;
	m_eye = m_ref + vecfront * distance;
}

void CCamera::UpdateUpVec()
{
	Vector3D vec = m_ref - m_eye;
	Vector3D zVec(0, 0, 1);
	Vector3D vec0;

	vec.SetUnit();
	vec0 = vec ^ zVec;
	m_vec_up = vec0 ^ vec;
}
