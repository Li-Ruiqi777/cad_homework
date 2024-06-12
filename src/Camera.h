#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector3D.h"

class Vector3D;

// 典型视图
enum ViewType
{
	VIEW_FRONT,
	VIEW_BACK,
	VIEW_TOP,
	VIEW_BOTTOM,
	VIEW_RIGHT,
	VIEW_LEFT,
	VIEW_ISOMETRIC,
};

class CCamera
{
private:
	// 绘制物体 gllookat
	Vector3D m_eye;
	Vector3D m_ref;
	Vector3D m_vec_up;

	// 画物体用视景体
	double m_far, m_near;
	double m_width, m_height;

	// 视口
	double m_screen[2];

public:
	CCamera();
	~CCamera();

	void Init();

	void ProjectionAndLookatForDisplay();
	void Selection(int xPos, int yPos);

	// 放大
	void Zoom(double scale);
	void ZoomAll(double x0, double y0, double z0, double x1, double y1, double z1);

	// 各种视图
	void SetViewType(ViewType type);

	// 移动
	void MoveView(double dx, double dy);

	// 旋转
	void RotateView(double a, double b);

	// 根据实际窗口设置视口
	void SetScreen(int x, int y);

	// GLlookat
	void SetEye(double eye_x, double eye_y, double eye_z);
	void SetRef(double ref_x, double ref_y, double ref_z);
	void SetVecUp(double up_dx, double up_dy, double up_dz);

	// 设置视口宽高
	void SetViewRect(double width, double height);
	void GetViewRect(double &width, double &height);

protected:
	void UpdateUpVec();
};
#endif
