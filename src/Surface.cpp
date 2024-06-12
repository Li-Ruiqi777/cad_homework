#include "surface.h"
#include "curve.h"

CSurface::CSurface(void)
{
	for (int uv = 0; uv < 2; uv++)
	{
		m_pointnum[uv] = 0;
		m_ctrlvertexnum[uv] = 0;
	}
}

CSurface::~CSurface()
{
}

// 计算二次Bezier曲面
void CSurface::BiQuadraticBezierSurface()
{
	float u, v, Bim[3], Bjn[3]; // 变量定义

	m_ctrlvertexnum[0] = 3; // u向控制顶点个数
	m_ctrlvertexnum[1] = 3; // v向控制顶点个数

	int ptnumoneseg = 21; // 每个方向用21个点来绘制，共20小段

	// 控制顶点
	float ctrlvertex[3][3][3] =
		{
			{
				{-100, 0, -100}, //[0][0][1~3],
				{0, 100, -100},	 //[0][1][1~3]
				{100, 0, -100}	 //[0][2][1~3]
			},					 // d0j
			{
				{-150, 40, 0}, //[1][0][1~3],
				{0, 150, 0},   //[1][1][1~3]
				{120, 0, 30}   //[1][2][1~3]
			},				   // d1j
			{
				{-100, 0, 100}, //[2][0][1~3],
				{0, 100, 100},	//[2][1][1~3]
				{100, 0, 100}	//[2][2][1~3]
			} // d2j
		};
	for (int i = 0; i < m_ctrlvertexnum[0]; i++) // u向控制顶点
	{
		for (int j = 0; j < m_ctrlvertexnum[1]; j++)
		{
			for (int xyz = 0; xyz < 3; xyz++) // x,y,z方向
				m_ctrlvertex[i][j][xyz] = ctrlvertex[i][j][xyz];
		}
	}

	// 曲面上的点初始化
	for (int ii = 0; ii < ptnumoneseg; ii++)
		for (int jj = 0; jj < ptnumoneseg; jj++)
			for (int xyz = 0; xyz < 3; xyz++)
			{
				m_point[ii][jj][xyz] = 0;
			}

	for (int ii = 0; ii < ptnumoneseg; ii++)
	{
		u = float(ii) / (ptnumoneseg - 1);
		for (int jj = 0; jj < ptnumoneseg; jj++)
		{
			v = float(jj) / (ptnumoneseg - 1);
			for (int i = 0; i < m_ctrlvertexnum[0]; i++) // u向控制顶点
			{
				Bim[i] = BernsteinBasis(i, m_ctrlvertexnum[0] - 1, u); // u向基函数
				for (int j = 0; j < m_ctrlvertexnum[1]; j++)
				{
					Bjn[j] = BernsteinBasis(j, m_ctrlvertexnum[1] - 1, v); // v向基函数
					for (int xyz = 0; xyz < 3; xyz++)					   // x,y,z方向
						m_point[ii][jj][xyz] += Bim[i] * Bjn[j] * ctrlvertex[i][j][xyz];
				}
			}
		}
	}

	m_pointnum[0] = ptnumoneseg; // 绘制点的个数存放起来
	m_pointnum[1] = ptnumoneseg;
}