#include <math.h>
#include <fstream>
#include <istream>
#include <sstream>
#include <string.h>
#include "curve.h"
#include "Vector3D.h"
#include <Eigen3/Eigen/Dense>

#define PI 3.1415926

CCurve::CCurve(void)
{
	m_pointnum = 0;
	m_ctrlvertexnum = 0;
	m_clicknum = 0;
	m_k = 0;
	m_piecenum = 0;
}

CCurve::~CCurve()
{
}

// 计算圆上点和切矢量
void CCurve::CircleAndTangentLine()
{
	int ptnumoneseg = 11;
	for (int i = 0; i < ptnumoneseg; i++)
	{
		m_pointx[i] = (float)cos(i * 2 * PI / (ptnumoneseg - 1)) * 100; // 圆的半径为100
		m_pointy[i] = (float)sin(i * 2 * PI / (ptnumoneseg - 1)) * 100;
		m_pointz[i] = 0;

		m_tangentx[i] = -(float)sin(i * 2 * PI / (ptnumoneseg - 1)); // 取单位长度
		m_tangenty[i] = (float)cos(i * 2 * PI / (ptnumoneseg - 1));
		m_tangentz[i] = 0;
	}
	m_pointnum = ptnumoneseg; // 绘制点的个数存放起来
}

// 计算三次曲线r(t)=(t,t^2,t^3)，t∈[0,1]
void CCurve::CubicPolynomialCurve()
{
	int ptnumoneseg = 51;
	Vector3D vec_tangent;
	float t;
	for (int i = 0; i < ptnumoneseg; i++)
	{
		t = (float)(i) / (ptnumoneseg - 1) * 2.0f - 1.0f;
		m_pointx[i] = t;
		m_pointy[i] = t * t;
		m_pointz[i] = t * t * t;

		vec_tangent[0] = 1.0;
		vec_tangent[1] = 2.0 * t;
		vec_tangent[2] = 3.0 * t * t;
		vec_tangent.SetUnit();

		m_tangentx[i] = (float)vec_tangent[0];
		m_tangenty[i] = (float)vec_tangent[1];
		m_tangentz[i] = (float)vec_tangent[2];
	}
	m_pointnum = ptnumoneseg;
}

// 计算二次Bezier曲线,r(u)=(1-u)^2b0+2(1-u)ub1+u^2b2
void CCurve::QuadraticBezierCurve()
{
	float u, B02, B12, B22; // 变量定义

	m_ctrlvertexnum = 3;
	int ptnumoneseg = 21; // 一段曲线用21个点来绘制，共20小段

	for (int j = 0; j < m_ctrlvertexnum; j++) // 控制顶点初始化
	{
		m_ctrlvertexx[j] = 0;
		m_ctrlvertexy[j] = 0;
		m_ctrlvertexz[j] = 0;
	}

	// 给定控制顶点坐标(0,0,0),(0,100,0),(100,100,0)
	m_ctrlvertexy[1] = 100;

	m_ctrlvertexx[2] = 100;
	m_ctrlvertexy[2] = 100;

	for (int i = 0; i < ptnumoneseg; i++)
	{
		u = float(i) / (ptnumoneseg - 1); // 参数，注意它是[0,1]之间的小数，因而只能用浮点数相除
		B02 = (1 - u) * (1 - u);		  // 基函数
		B12 = 2.0f * (1 - u) * u;
		B22 = u * u;

		m_pointx[i] = B02 * m_ctrlvertexx[0] + B12 * m_ctrlvertexx[1] + B22 * m_ctrlvertexx[2]; // 曲线上的点
		m_pointy[i] = B02 * m_ctrlvertexy[0] + B12 * m_ctrlvertexy[1] + B22 * m_ctrlvertexy[2];
		m_pointz[i] = B02 * m_ctrlvertexz[0] + B12 * m_ctrlvertexz[1] + B22 * m_ctrlvertexz[2];
	}
	m_pointnum = ptnumoneseg; // 绘制点的个数存放起来
}

void CCurve::QuadraticBezierCurveByClick()
{
	float u, B02, B12, B22; // 变量定义

	m_ctrlvertexnum = 3;
	int ptnumoneseg = 21; // 一段曲线用21个点来绘制，共20小段

	for (int i = 0; i < ptnumoneseg; i++)
	{
		u = float(i) / (ptnumoneseg - 1); // 参数，注意它是[0,1]之间的小数，因而只能用浮点数相除
		B02 = (1 - u) * (1 - u);		  // 基函数
		B12 = 2.0f * (1 - u) * u;
		B22 = u * u;

		m_pointx[i] = B02 * m_ctrlvertexx[0] + B12 * m_ctrlvertexx[1] + B22 * m_ctrlvertexx[2]; // 曲线上的点
		m_pointy[i] = B02 * m_ctrlvertexy[0] + B12 * m_ctrlvertexy[1] + B22 * m_ctrlvertexy[2];
		m_pointz[i] = B02 * m_ctrlvertexz[0] + B12 * m_ctrlvertexz[1] + B22 * m_ctrlvertexz[2];
	}
	m_pointnum = ptnumoneseg; // 绘制点的个数存放起来
}

// 从文件中读取Bezier曲线次数、条数及控制顶点
// 读入m_k,m_piecenum,m_ctrlvertexnum,m_ctrlvertexx[],m_ctrlvertexy[],m_ctrlvertexz[]
void CCurve::ReadBezierCurveControlVertices(const char *filenamestr)
{
	std::ifstream infile;	  // 文件流
	std::string lineread;	  // 读入的一行字符串
	std::string tmpstr;		  // 某个字符串
	std::stringstream strstr; // 处理字符串的流

	int valueline = 0;
	int indexctrlvertex = 0;
	int ixyz = 0;

	infile.open(filenamestr); // 打开文件
	if (infile.is_open())
	{
		while (std::getline(infile, lineread))
		{
			if (lineread != "")
			{
				if (lineread[0] != '#') // 如果一行中第一个字符是#号，则不进行处理
				{

					if (valueline == 0) // 第一个有效行读入曲线的次数
					{
						m_k = std::stoi(lineread);
					}
					else if (valueline == 1) // 第二个有效行读入曲线的条数
					{
						m_piecenum = std::stoi(lineread);
					}
					else if (valueline >= 2)
					{
						strstr.str(lineread);
						ixyz = 0;
						while (std::getline(strstr, tmpstr, ','))
						{ // 按“，”隔开字符串
							if (ixyz % 3 == 0)
								m_ctrlvertexx[indexctrlvertex] = (float)std::stod(tmpstr); // stod: string->float
							if (ixyz % 3 == 1)
								m_ctrlvertexy[indexctrlvertex] = (float)std::stod(tmpstr);
							if (ixyz % 3 == 2)
								m_ctrlvertexz[indexctrlvertex] = (float)std::stod(tmpstr);
							ixyz++;
						}
						strstr.clear();
						indexctrlvertex++;
					}
					valueline++;
				}
			}
		}
		m_ctrlvertexnum = indexctrlvertex;
		infile.close();
	}
	else
	{
		printf("The Bezier curve file is not opened.\n");
	}
}

// 计算多条Bezier曲线
void CCurve::QuadraticBezierCurveNPieces()
{
	float u, B02, B12, B22; // 变量定义
	int ptnumoneseg = 21;	// 一段曲线用21个点来绘制，共20小段

	// 首先判断一下是否满足计算的条件
	if (m_ctrlvertexnum == 0)
	{
		printf("No control vertices for creating the Bezier curve.\n");
		return;
	}
	if (m_ctrlvertexnum < (m_k + 1) * m_piecenum)
	{
		printf("Control vertices is not enough.\n");
		return;
	}

	if (m_k == 2)
	{
		for (int ipiece = 0; ipiece < m_piecenum; ipiece++)
		{
			for (int i = 0; i < ptnumoneseg; i++)
			{
				u = float(i) / (ptnumoneseg - 1); // 参数，注意它是[0,1]之间的小数，因而只能用浮点数相除
				B02 = (1 - u) * (1 - u);		  // 基函数
				B12 = 2.0f * (1 - u) * u;
				B22 = u * u;

				m_pointx[ipiece * ptnumoneseg + i] = B02 * m_ctrlvertexx[ipiece * (m_k + 1) + 0] + B12 * m_ctrlvertexx[ipiece * (m_k + 1) + 1] + B22 * m_ctrlvertexx[ipiece * (m_k + 1) + 2]; // 曲线上的点
				m_pointy[ipiece * ptnumoneseg + i] = B02 * m_ctrlvertexy[ipiece * (m_k + 1) + 0] + B12 * m_ctrlvertexy[ipiece * (m_k + 1) + 1] + B22 * m_ctrlvertexy[ipiece * (m_k + 1) + 2];
				m_pointz[ipiece * ptnumoneseg + i] = B02 * m_ctrlvertexz[ipiece * (m_k + 1) + 0] + B12 * m_ctrlvertexz[ipiece * (m_k + 1) + 1] + B22 * m_ctrlvertexz[ipiece * (m_k + 1) + 2];
			}
		}
	}
	m_pointnum = m_piecenum * ptnumoneseg; // 绘制点的个数存放起来
}

// De Casteljau算法计算Bezier曲线上的点，x,y,z三个坐标上分别计算
// degree是次数，coeff[]是控制顶点某个坐标分量，u是参数
float CCurve::decas(int degree, float coeff[], float u)
{
	int r, i;
	float u1;
	float coeffa[200];
	u1 = 1.0f - u;
	for (i = 0; i <= degree; i++)
		coeffa[i] = coeff[i]; /*save input array*/
	for (r = 1; r <= degree; r++)
		for (i = 0; i <= degree - r; i++)
		{
			coeffa[i] = u1 * coeffa[i] + u * coeffa[i + 1];
		}
	return (coeffa[0]);
}

void CCurve::DecasteljauBezierCurve()
{
	// 首先判断一下是否满足计算的条件
	if (m_ctrlvertexnum == 0)
	{
		printf("No control vertices for creating the Bezier curve.\n");
		return;
	}
	if (m_ctrlvertexnum < (m_k + 1) * m_piecenum)
	{
		printf("Control vertices is not enough.\n");
		return;
	}

	float u, coeffx[200], coeffy[200], coeffz[200];
	int ptnumoneseg = 21;
	for (int ipiece = 0; ipiece < m_piecenum; ipiece++)
	{
		// 每一段曲线的控制顶点个数为m_k + 1个
		for (int ictrlvertex = 0; ictrlvertex < m_k + 1; ictrlvertex++)
		{
			coeffx[ictrlvertex] = m_ctrlvertexx[ipiece * (m_k + 1) + ictrlvertex];
			coeffy[ictrlvertex] = m_ctrlvertexy[ipiece * (m_k + 1) + ictrlvertex];
			coeffz[ictrlvertex] = m_ctrlvertexz[ipiece * (m_k + 1) + ictrlvertex];
		}
		for (int i = 0; i < ptnumoneseg; i++)
		{
			u = float(i) / (ptnumoneseg - 1);
			m_pointx[ipiece * ptnumoneseg + i] = decas(m_k, coeffx, u);
			m_pointy[ipiece * ptnumoneseg + i] = decas(m_k, coeffy, u);
			m_pointz[ipiece * ptnumoneseg + i] = decas(m_k, coeffz, u);
		}
	}
	m_pointnum = m_piecenum * ptnumoneseg; // 绘制点的个数存放起来
}

// 阶乘计算
unsigned long Factorial(unsigned int n)
{
	if (n == 0)
		return 1;
	unsigned long value = n;
	for (n--; n > 0; n--)
		value *= n;
	return value;
}

// 组合数计算
unsigned long Combination(unsigned int i, unsigned int n)
{
	if (i > n)
		return 0;
	return Factorial(n) / (Factorial(i) * Factorial(n - i));
}

// Bernstein基函数计算
double BernsteinBasis(unsigned int i, unsigned int n, float u)
{
	return Combination(i, n) * pow(u, i) * pow(1.0 - u, n - i);
}

// Bezier曲线插值计算控制顶点
// 采用累积弦长计算参数
void InverseComputeBezier(int degree, float datapt[][3], float ctrlvertex[][3])
{
	float udata[200]; // 型值点的参数
	float l[200];	  // 弦长

	// 先用累积弦长计算曲线参数
	for (int idata = 0; idata <= degree; idata++)
	{
		if (idata == 0)
		{
			l[idata] = 0;
		}
		else
		{
			l[idata] = l[idata - 1] + sqrt((datapt[idata][0] - datapt[idata - 1][0]) * (datapt[idata][0] - datapt[idata - 1][0]) + (datapt[idata][1] - datapt[idata - 1][1]) * (datapt[idata][1] - datapt[idata - 1][1]) + (datapt[idata][2] - datapt[idata - 1][2]) * (datapt[idata][2] - datapt[idata - 1][2]));
		}
	}
	for (int idata = 0; idata <= degree; idata++)
	{
		udata[idata] = l[idata] / l[degree];
	}

	// 写成AX=b的形式，构造插值矩阵A，用Eigen库来计算逆矩阵A-
	// x,y,z分别算，但A是一样的，b有三个
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> x;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> b;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> AInv;
	// 各元素初始化为0
	A = Eigen::MatrixXd::Zero(degree + 1, degree + 1);
	b = Eigen::MatrixXd::Zero(degree + 1, 3);
	// Bernstein基函数构成的插值矩阵
	for (int i = 0; i <= degree; i++)
		for (int j = 0; j <= degree; j++)
		{
			A(i, j) = BernsteinBasis(j, degree, udata[i]);
		}
	for (int i = 0; i <= degree; i++)
		for (int k = 0; k <= 2; k++)
		{
			b(i, k) = datapt[i][k];
		}
	AInv = A.inverse();
	x = AInv * b;
	for (int k = 0; k <= 2; k++)
	{
		for (int i = 0; i <= degree; i++)
		{
			ctrlvertex[i][k] = (float)x(i, k);
		}
	}
}

// 用Bezier曲线插值来拟合1/4圆
void CCurve::InterpolateArcByBezierCurve(int degree)
{
	float datapt[200][3], ctrlvertex[200][3];

	// 在1/4圆上采样型值点m_datapointx,m_datapointy,m_datapointz
	for (int i = 0; i <= degree; i++)
	{
		m_datapointx[i] = (float)cos(i * PI / 2 / degree) * 300 - 150;
		m_datapointy[i] = (float)sin(i * PI / 2 / degree) * 300 - 150;
		m_datapointz[i] = 0;

		datapt[i][0] = m_datapointx[i];
		datapt[i][1] = m_datapointy[i];
		datapt[i][2] = m_datapointz[i];
	}

	// 反算控制顶点
	InverseComputeBezier(degree, datapt, ctrlvertex);

	for (int i = 0; i <= degree; i++)
	{
		m_ctrlvertexx[i] = ctrlvertex[i][0];
		m_ctrlvertexy[i] = ctrlvertex[i][1];
		m_ctrlvertexz[i] = ctrlvertex[i][2];
	}

	m_k = degree;
	m_piecenum = 1;
	m_datapointnum = degree + 1;
	m_ctrlvertexnum = degree + 1;
}

// 从鼠标点击中获得控制顶点，计算三次均匀B样条曲线各段上的点
void CCurve::CubicBsplineCurveByClick()
{
	float u, N03, N13, N23, N33; // 变量定义
	int ptnumoneseg = 21;		 // 一段曲线提高到用21个点来绘制，共20小段
	int segmentnum = m_clicknum - 3;
	for (int iseg = segmentnum - 1; iseg < segmentnum; iseg++)
	{
		for (int i = 0; i < ptnumoneseg; i++)
		{
			u = float(i) / (ptnumoneseg - 1);
			N03 = 1.0f / 6 * (-u * u * u + 3 * u * u - 3 * u + 1);
			N13 = 1.0f / 6 * (3 * u * u * u - 6 * u * u + 4);
			N23 = 1.0f / 6 * (-3 * u * u * u + 3 * u * u + 3 * u + 1);
			N33 = 1.0f / 6 * u * u * u;

			m_pointx[iseg * ptnumoneseg + i] = N03 * m_ctrlvertexx[iseg + 0] + N13 * m_ctrlvertexx[iseg + 1] + N23 * m_ctrlvertexx[iseg + 2] + N33 * m_ctrlvertexx[iseg + 3];
			m_pointy[iseg * ptnumoneseg + i] = N03 * m_ctrlvertexy[iseg + 0] + N13 * m_ctrlvertexy[iseg + 1] + N23 * m_ctrlvertexy[iseg + 2] + N33 * m_ctrlvertexy[iseg + 3];
			m_pointz[iseg * ptnumoneseg + i] = N03 * m_ctrlvertexz[0] + N13 * m_ctrlvertexz[iseg + 1] + N23 * m_ctrlvertexz[iseg + 2] + N33 * m_ctrlvertexz[iseg + 3];
		}
	}
	m_pointnum = segmentnum * ptnumoneseg; // 绘制点的个数
}

// 从文件中读取B样条曲线条数，每条曲线的控制顶点数、次数、节点矢量和控制顶点坐标
// 读入m_piecenum,m_piecectrlvertexnum[],m_piecek[],m_piecesegmentnum[]
// m_knot[],m_ctrlvertexx[],m_ctrlvertexy[],m_ctrlvertexz[],m_ctrlvertexnum
void CCurve::ReadBsplineCurveControlVertices(const char *filenamestr)
{
	std::ifstream infile;	  // 文件流
	std::string lineread;	  // 读入的一行字符串
	std::string tmpstr;		  // 某个字符串
	std::stringstream strstr; // 处理字符串的流

	int valueline = 0; // 文件中有效行计数，出现一个126则置零
	int piece = 0;	   // 曲线的条数，每个126后面跟一条
	int segment = 0;   // 每条曲线的段数
	int iseparate = 0;
	int n = 0;				 // 一条曲线中控制顶点的个数
	int k = 0;				 // 一条曲线的曲线次数
	int m = 0;				 // 一条曲线中节点矢量的个数
	int indexctrlvertex = 0; // 控制顶点的序号
	int indexknot = 0;		 // 节点矢量的序号

	infile.open(filenamestr); // 打开文件

	// 与Bezier控制顶点文件读取程序的写法有些不同，这里为大家阅读方便
	// 尽量减少if程序的嵌套数,而采用return、continue的方法
	if (!infile.is_open())
	{
		printf("BSpline file is not found.\n");
		return;
	}

	while (std::getline(infile, lineread))
	{
		if (lineread == "") // 回车键不处理
			continue;
		if (lineread[0] == '#') // 如果一行中第一个字符是#号，则不进行处理
			continue;
		if (lineread == "126") // 出现一个126标志，则开始一条B样条曲线,将有效行置为0
		{
			valueline = 0;
			piece++;
		}
		if (valueline == 1) // 第二个有效行读入n和k
		{
			strstr.str(lineread);
			iseparate = 0;
			while (std::getline(strstr, tmpstr, ','))
			{ // 按“，”隔开字符串
				if (iseparate == 0)
					n = std::stoi(tmpstr);
				else if (iseparate == 1)
					k = std::stoi(tmpstr);
				iseparate++;
			}
			m_piecectrlvertexnum[piece - 1] = n + 1;  // 每条曲线控制顶点数
			m_piecek[piece - 1] = k;				  // 每条曲线次数
			m_piecesegmentnum[piece - 1] = n - k + 1; // 每条曲线的段数
			m_ctrlvertexnum += n + 1;				  // 总的控制顶点个数
			strstr.clear();
		}
		else if (valueline == 2) // 第三个有效行读入knot，个数应该为m=n+k+1，这里就不检查是否个数是否符合要求了
		{
			strstr.str(lineread);
			iseparate = 0;
			while (std::getline(strstr, tmpstr, ','))
			{
				m_knot[indexknot] = (float)std::stod(tmpstr);
				indexknot++;
			}
			strstr.clear();
		}
		else if (valueline >= 3) // 第四个及以上有效行读入控制顶点
		{
			strstr.str(lineread);
			iseparate = 0;
			while (std::getline(strstr, tmpstr, ','))
			{ // 按“，”隔开字符串
				if (iseparate % 3 == 0)
					m_ctrlvertexx[indexctrlvertex] = (float)std::stod(tmpstr); // stod: string->float
				if (iseparate % 3 == 1)
					m_ctrlvertexy[indexctrlvertex] = (float)std::stod(tmpstr);
				if (iseparate % 3 == 2)
					m_ctrlvertexz[indexctrlvertex] = (float)std::stod(tmpstr);
				iseparate++;
			}
			strstr.clear();
			indexctrlvertex++;
		}
		valueline++;
	}
	m_piecenum = piece;
	infile.close();
}

// deBoor算法
// degree：degree of B-spline
// knot: knot sequence
// CtrlVertex: control Vertices
// u: parameter value
// ik: integer such that knot[ik] <= u < knot[ik+1]
// R: r(u)
void CCurve::CoxdeBoor(int degree, float knot[], float CtrlVertex[], float u, int ik, float *R)
{
	float A[200];
	int i, j, r;
	double d1, d2;

	for (j = 0; j <= degree; j++)
		A[j] = CtrlVertex[ik - degree + j];
	for (r = 1; r <= degree; r++)
	{
		for (j = degree; j >= r; j--)
		{
			i = ik - degree + j;
			d1 = u - knot[i];
			d2 = knot[i + degree + 1 - r] - u;
			A[j] = (float)((d1 * A[j] + d2 * A[j - 1]) / (d1 + d2));
		}
	}
	*R = A[degree];
}

// 调用deBoor算法CoxdeBoor()计算B样条曲线
void CCurve::CoxdeBoorBSplineCurve()
{
	float u, knot[200], ctrlvertexx[200], ctrlvertexy[200], ctrlvertexz[200];
	int ik;
	float rx, ry, rz;
	int ptnumoneseg = 21;
	int n, k, m;
	int mlastpiece = -1;								// 为便于计算当前这条曲线的节点矢量序号，保存上一条曲线的节点矢量
	int nlastpiece = -1;								// 保存上一条曲线的控制顶点
	int piecesegmentnum = 0;							// 所有曲线条数的段数总和
	for (int ipiece = 0; ipiece < m_piecenum; ipiece++) // 曲线的条数
	{
		n = m_piecectrlvertexnum[ipiece] - 1;
		k = m_piecek[ipiece];
		m = n + k + 1;
		// 这一条曲线的控制顶点x,y,z
		for (int ictrlvertex = 0; ictrlvertex <= n; ictrlvertex++)
		{
			ctrlvertexx[ictrlvertex] = m_ctrlvertexx[nlastpiece + 1 + ictrlvertex];
			ctrlvertexy[ictrlvertex] = m_ctrlvertexy[nlastpiece + 1 + ictrlvertex];
			ctrlvertexz[ictrlvertex] = m_ctrlvertexz[nlastpiece + 1 + ictrlvertex];
		}
		// 这一条曲线的节点矢量knot
		for (int iknot = 0; iknot <= m; iknot++)
		{
			knot[iknot] = m_knot[mlastpiece + 1 + iknot]; // mlastpiece需要从-1开始
		}

		for (int isegment = 0; isegment < m_piecesegmentnum[ipiece]; isegment++) // 每一条曲线的段数
		{
			ik = isegment + k; // 这一段曲线的定义域是knot[ik]到knot[ik+1]，ik=isegment+k
			for (int i = 0; i < ptnumoneseg; i++)
			{
				u = knot[ik] + float(i) / (ptnumoneseg - 1) * (knot[ik + 1] - knot[ik]);
				CoxdeBoor(k, knot, ctrlvertexx, u, ik, &rx);
				CoxdeBoor(k, knot, ctrlvertexy, u, ik, &ry);
				CoxdeBoor(k, knot, ctrlvertexz, u, ik, &rz);
				m_pointx[piecesegmentnum * ptnumoneseg + i] = rx;
				m_pointy[piecesegmentnum * ptnumoneseg + i] = ry;
				m_pointz[piecesegmentnum * ptnumoneseg + i] = rz;
			}
			piecesegmentnum++;
		}
		// 更新mlastpiece,nlastpiece
		mlastpiece += m + 1;
		nlastpiece += n + 1;
	}
	m_pointnum = piecesegmentnum * ptnumoneseg; // 曲线段数乘以每段的点数
}