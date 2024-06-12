#pragma once

class CCurve;
class CSurface;

void DrawScene(int iScene);
void DrawScene(int iScene, CCurve *pCurve);

void DrawteTrahedron();
void DrawCurveAndTangentLine(CCurve *m_pCurve);
void DrawBezierCurve(CCurve *m_pCurve);
void DrawBezierControlPolygon(CCurve *m_pCurve);

void DrawSurface(CSurface *m_pSurface);
void DrawSurfaceControlPolygon(CSurface *m_pSurface);