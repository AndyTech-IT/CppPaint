#include "api.h"

#ifndef Point
struct Point
{
	double x, y;
};
#endif 

#ifndef Color
struct Color
{
	int R, G, B;
};
#endif

#ifndef Brush_Type
enum class Brush_Type
{
	Square,
	Circle,
};
#endif


#ifndef Brush
struct Brush
{
	double Size;
	Color Color;
	Brush_Type Type;
};
#endif

#ifndef Vidget
struct Vidget {
public:
	Point Position;
	Point Size;
	Color BaseColor;
	Color SelectedColor;
	Vidget(Point pos, Point size, Color base, Color select) {
		Position = pos;
		Size = size;
		BaseColor = base;
		SelectedColor = select;
	}
};
#endif // !Vidget


#ifndef Engine
class Engine 
{
	GLsizei Height = 2000;
	GLsizei Width = 2000;

	double WinWidth;
	double WinHeight;
	double WidthScale;
	double HeightScale;

	const double ZMax = 100;

	Brush CurentBrush = Brush{
		10,
		Color{ 0,0,0 },
		Brush_Type::Circle
	};;

	int CurentVidgetID = -1;
	Vidget* Vidgets;
	int VidgetsCount;

	Point LastPoint = Point{ -1, -1 };

	GLvoid draw_rect(Point pos, Point size, Color c);

public:
	GLvoid draw_point(Point p);

	GLvoid Resize(GLsizei width, GLsizei height); // �������, ���������� ��� ��������� �������� ����
	GLvoid Init(GLvoid); // �������, ��� ������� ��������� ����������
	GLvoid Draw(GLvoid); // ��������� (render) ����� 
	int SetWindowPixelFormat(HDC hDC); //�������, ������� ������������� ��������� ��������� ��������������� OpenGL 

	GLvoid MouseDown(Point p, bool Shift);
	GLvoid MouseDrag(Point begin, Point end);
};
#endif


