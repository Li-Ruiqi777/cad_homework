
#ifndef __VECTOR_H
#define __VECTOR_H

#include <cmath>
#define TOLER 1e-6

class Vector3D
{
public:
	double x, y, z;

public:
	void SetZero();
	// 构造函数
	Vector3D()
	{
		x = y = z = 0;
	}
	Vector3D(double newx, double newy, double newz)
	{
		x = newx;
		y = newy;
		z = newz;
	}
	virtual Vector3D *Clone();

	// 赋值操作符
	void set(const Vector3D *v)
	{
		x = v->x;
		y = v->y;
		z = v->z;
	}
	void set(double newx, double newy, double newz)
	{
		x = newx;
		y = newy;
		z = newz;
	}

	//	void operator  = (const Vector3D& v)
	//		{x = v.x; y = v.y; z = v.z;}

	void operator+=(const Vector3D &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator-=(const Vector3D &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	void operator*=(double num)
	{
		x *= num;
		y *= num;
		z *= num;
	}
	void operator*=(Vector3D v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}
	void operator/=(double num)
	{ // ASSERT (fabs (num) < TOLER);
		x /= num;
		y /= num;
		z /= num;
	}
	void operator^=(const Vector3D &v)
	{
		double a = y * v.z - z * v.y;
		double b = -x * v.z + z * v.x;
		double c = x * v.y - y * v.x;

		x = a;
		y = b;
		z = c;
	}

	Vector3D GetNormal()
	{
		Vector3D temp(*this);
		temp.SetUnit();
		return temp;
	}

	// 取最大值的索引
	int maxi();
	// 取最大值
	double maxvalue();
	// 矢量加
	Vector3D operator+(const Vector3D &v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}
	// 矢量减
	Vector3D operator-(const Vector3D &v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}
	// 矢量乘(点积)
	double operator*(const Vector3D &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3D operator%(const Vector3D &v) const
	{
		return Vector3D(x * v.x, y * v.y, z * v.z);
	}
	// 矢量乘(叉积)
	Vector3D operator^(const Vector3D &v) const
	{
		return Vector3D(y * v.z - z * v.y,
						-x * v.z + z * v.x,
						x * v.y - y * v.x);
	}
	// 数乘
	Vector3D operator*(double num) const
	{
		return Vector3D(x * num, y * num, z * num);
	}
	// 数除
	Vector3D operator/(double num) const
	{ // ASSERT (fabs (num) < TOLER);
		return Vector3D(x / num, y / num, z / num);
	}
	// 单目减
	Vector3D operator-() const
	{
		return Vector3D(-x, -y, -z);
	}

	// 判等(不等)
	int operator==(const Vector3D &v) const
	{
		return (disFrom(v) < TOLER);
	}
	int operator!=(const Vector3D &v) const { return disFrom(v) >= TOLER; }

	// 取元素
	double &operator[](int i)
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return x;
		}
	}

	// 单位化
	void SetUnit()
	{
		double len = GetLen();
		if (len < TOLER)
		{
			return;
		}
		x /= len;
		y /= len;
		z /= len;
	}
	// 取长度
	double GetLen() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	double SquLen() const
	{
		return (x * x + y * y + z * z);
	}
	double disFrom(const Vector3D &vec) const;
	void rotateX(double pitch);
	void rotateY(double yaw);
	void rotateZ(double roll);

	void getRotMatrix(double *out, double radians) const;
	void rotate(Vector3D &target, double radians) const;
	void SetLen(double new_m);
	void rotate(double add_theta, double add_phi);
	double getPhi(void) const;
	double getTheta(void) const;
};

#endif
