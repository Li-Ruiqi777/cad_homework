#include "Vector3D.h"
#include <stdio.h>
#include <tchar.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double Vector3D::disFrom(const Vector3D &vec) const
{
	return (double)((vec.x - x) * (vec.x - x) +
					(vec.y - y) * (vec.y - y) +
					(vec.z - z) * (vec.z - z));
}

void Vector3D::rotateX(double pitch)
{

	double angle = (double)((double)pitch * M_PI / 180.0f);
	double sina = sin(angle);
	double cosa = cos(angle);

	double tmpy = (y * cosa) - (z * sina);
	double tmpz = (y * sina) + (z * cosa);

	this->y = tmpy;
	this->z = tmpz;
}

void Vector3D::rotateY(double yaw)
{

	double angle = (double)((double)yaw * M_PI / 180.0f);
	double sina = sin(angle);
	double cosa = cos(angle);

	double tmpx = (x * cosa) + (z * sina);
	double tmpz = (z * cosa) - (x * sina);

	this->x = tmpx;
	this->z = tmpz;
}

void Vector3D::rotateZ(double roll)
{

	double angle = (double)((double)roll * M_PI / 180.0f);
	double sina = sin(angle);
	double cosa = cos(angle);

	double tmpx = (x * cosa) - (y * sina);
	double tmpy = (y * cosa) + (x * sina);

	this->x = tmpx;
	this->y = tmpy;
}

void Vector3D::getRotMatrix(double *out, double radians) const
{

	// This function contributed by Erich Boleyn (erich@uruk.org).
	// This function used from the Mesa OpenGL code (matrix.c).

	register double mag, s, c;
	register double vx, vy, vz, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin(radians);
	c = cos(radians);

	mag = GetLen();

	vx = this->x / mag;
	vy = this->y / mag;
	vz = this->z / mag;

#define M(row, col) out[row * 3 + col]

	// Arbitrary axis rotation matrix.

	// This is composed of 5 matrices, Rz, Ry, double, Ry', Rz', multiplied
	// like so:  Rz * Ry * double * Ry' * Rz'.  double is the final rotation
	// (which is about the X-axis), and the two composite transforms
	// Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
	// from the arbitrary axis to the X-axis then back.  They are
	// all elementary rotations.

	// Rz' is a rotation about the Z-axis, to bring the axis vector
	// into the x-z plane.  Then Ry' is applied, rotating about the
	// Y-axis to bring the axis vector parallel with the X-axis.  The
	// rotation about the X-axis is then performed.  Ry and Rz are
	// simply the respective inverse transforms to bring the arbitrary
	// axis back to it's original orientation.  The first transforms
	// Rz' and Ry' are considered inverses, since the data from the
	// arbitrary axis gives you info on how to get to it, not how
	// to get away from it, and an inverse must be applied.

	// The basic calculation used is to recognize that the arbitrary
	// axis vector (x, y, z), since it is of unit GetLen, actually
	// represents the sines and cosines of the angles to rotate the
	// X-axis to the same orientation, with theta being the angle about
	// Z and phi the angle about Y (in the order described above)
	// as follows:

	// cos ( theta ) = x / sqrt ( 1 - z^2 )
	// sin ( theta ) = y / sqrt ( 1 - z^2 )

	// cos ( phi ) = sqrt ( 1 - z^2 )
	// sin ( phi ) = z

	// Note that cos ( phi ) can further be inserted to the above
	// formulas:

	// cos ( theta ) = x / cos ( phi )
	// sin ( theta ) = y / cos ( phi )

	// ...etc.  Because of those relations and the standard trigonometric
	// relations, it is pssible to reduce the transforms down to what
	// is used below.  It may be that any primary axis chosen will give the
	// same results (modulo a sign convention) using thie method.

	// Particularly nice is to notice that all divisions that might
	// have caused trouble when parallel to certain planes or
	// axis go away with care paid to reducing the expressions.
	// After checking, it does perform correctly under all cases, since
	// in all the cases of division where the denominator would have
	// been zero, the numerator would have been zero as well, giving
	// the expected result.

	xx = vx * vx;
	yy = vy * vy;
	zz = vz * vz;
	xy = vx * vy;
	yz = vy * vz;
	zx = vz * vx;
	xs = vx * s;
	ys = vy * s;
	zs = vz * s;
	one_c = 1.0f - c;

	M(0, 0) = (one_c * xx) + c;
	M(1, 0) = (one_c * xy) - zs;
	M(2, 0) = (one_c * zx) + ys;

	M(0, 1) = (one_c * xy) + zs;
	M(1, 1) = (one_c * yy) + c;
	M(2, 1) = (one_c * yz) - xs;

	M(0, 2) = (one_c * zx) - ys;
	M(1, 2) = (one_c * yz) + xs;
	M(2, 2) = (one_c * zz) + c;

#undef M
}

// Note: 'angle' is in radians.

void Vector3D::rotate(Vector3D &target, double radians) const
{

	register double m[9];
	getRotMatrix(m, radians);

	register double nx, ny, nz, tx, ty, tz;

	tx = target.x;
	ty = target.y;
	tz = target.z;

#define M(row, col) m[row * 3 + col]

	nx = tx * M(0, 0) + ty * M(1, 0) + tz * M(2, 0);
	ny = tx * M(0, 1) + ty * M(1, 1) + tz * M(2, 1);
	nz = tx * M(0, 2) + ty * M(1, 2) + tz * M(2, 2);

#undef M

	target.x = nx;
	target.y = ny;
	target.z = nz;
}

void Vector3D::SetLen(double new_m)
{

	SetUnit();
	this->x *= new_m;
	this->y *= new_m;
	this->z *= new_m;
}

void Vector3D::rotate(double add_theta, double add_phi)
{

	register double phi = getPhi() + add_phi;
	register double theta = getTheta() + add_theta;
	register double mag = GetLen();

	// Normalize the new value.
	while (phi >= 2.0f * (double)M_PI)
		phi -= 2.0f * (double)M_PI;

	while (phi < 0)
		phi += 2.0f * (double)M_PI;

	// Normalize the new value.
	while (theta >= 2.0f * (double)M_PI)
		theta -= 2.0f * (double)M_PI;

	while (theta < 0)
		theta += 2.0f * (double)M_PI;

	this->x = (double)(mag * sin(phi) *
					   cos(theta));

	this->y = (double)(mag * sin(phi) *
					   sin(theta));

	this->z = (double)(mag * cos(phi));
}

double Vector3D::getPhi(void) const
{

	// Quick check to prevent divide by 0.
	register double mag = GetLen();
	if (mag * mag < 0.001f)
		return 0.0;

	return (double)acos(this->z / GetLen());
}

double Vector3D::getTheta(void) const
{

	// Quick check to prevent divide by 0.
	if (this->x * this->x + this->y * this->y < 0.0001)
		return 0;

	register double r = (double)sqrt(this->x * this->x + this->y * this->y);
	register double theta = (double)acos(this->x / r);

	// We want it from 0->2PI, not -PI->PI, so make sure to wrap it right.
	if (this->y < 0)
		theta = 2.0f * (double)M_PI - theta;

	return theta;
}

int Vector3D::maxi()
{
	int maxi = 0;
	double maxd = fabs(x);
	if (maxd < fabs(y))
	{
		maxi = 2;
		maxd = fabs(y);
	}
	if (maxd < fabs(z))
	{
		maxi = 3;
	}
	return maxi;
}

double Vector3D::maxvalue()
{
	double maxd = x;
	if (fabs(maxd) < fabs(y))
	{
		maxd = y;
	}
	if (fabs(maxd) < fabs(z))
	{
		maxd = z;
	}
	return maxd;
}
Vector3D *Vector3D::Clone()
{
	Vector3D *vector = new Vector3D(x, y, z);
	return vector;
}

void Vector3D::SetZero()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}
