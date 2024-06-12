#pragma once
class CSurface
{
public:
	int m_pointnum[2];				 //<=500,500  曲面uv向离散后的点个数
	float m_point[500][500][3];		 // 曲面uv向离散后的点位置
	float m_normal[500][500][3];	 // 曲面上各点的法矢量
	int m_ctrlvertexnum[2];			 //<=200,200 uv向控制顶点的个数
	float m_ctrlvertex[200][200][3]; // uv向的控制顶点位置

	int m_k[2]; // 曲面uv向的次数
public:
	CSurface();
	~CSurface();

	void BiQuadraticBezierSurface();
};