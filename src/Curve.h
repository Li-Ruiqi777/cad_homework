#ifndef _CURVE_H_
#define _CURVE_H_

class CCurve
{
public:
	int m_pointnum;													  //<=1000  曲线离散后的点个数
	float m_pointx[1000], m_pointy[1000], m_pointz[1000];			  // 曲线离散后的点位置
	float m_tangentx[1000], m_tangenty[1000], m_tangentz[1000];		  // 切线
	int m_ctrlvertexnum;											  //<=200 控制顶点的个数
	float m_ctrlvertexx[200], m_ctrlvertexy[200], m_ctrlvertexz[200]; // 曲线的控制顶点位置
	float m_datapointx[200], m_datapointy[200], m_datapointz[200];	  // 曲线的型值点位置
	int m_clicknum;													  // 鼠标点击的次数
	int m_k;														  // 曲线的次数
	int m_piecenum;													  // 曲线的条数
	int m_datapointnum;												  // 型值点的个数

	int m_piecectrlvertexnum[100]; // BSpline每条曲线的控制顶点个数
	int m_piecek[100];			   // BSpline每条曲线的次数
	int m_piecesegmentnum[100];	   // BSpline每条曲线的段数=n-k+1
	float m_knot[200];			   // BSpline节点矢量，所有条数曲线的节点矢量按照顺序来存放
public:
	CCurve();
	~CCurve();

	void CircleAndTangentLine();
	void CubicPolynomialCurve();

	void QuadraticBezierCurve();
	void CubicBezierCurve();
	void QuadraticBezierCurveByClick();
	void ReadBezierCurveControlVertices(const char *filenamestr);
	void QuadraticBezierCurveNPieces();

	float decas(int degree, float coeff[], float u);
	void DecasteljauBezierCurve();
	void InterpolateArcByBezierCurve(int degree);

	// Bspline
	void CubicBsplineCurveByClick();
	void ReadBsplineCurveControlVertices(const char *filenamestr);
	void CoxdeBoor(int degree, float knot[], float CtrlVertex[], float u, int ik, float *R);
	void CoxdeBoorBSplineCurve();

public:
};

unsigned long Factorial(unsigned int n);
unsigned long Combination(unsigned int i, unsigned int n);
double BernsteinBasis(unsigned int i, unsigned int n, float u);
void InverseComputeBezier(int degree, float datapt[][3], float ctrlvertex[][3]);

#endif