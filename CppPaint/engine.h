//
// ������������ ����, �������� ��� �������� ������
// ���������� ������� ���������� ��� � engine.cpp
//

#include "api.h"

#pragma region Enums

#ifndef Brush_Type
// ��� �����
enum class Brush_Type
{
	Square,
	Circle,
};
#endif

#ifndef ButtonStatus
// ������ ������
enum class ButtonStatus 
{
	Unselected,
	MouseHower,
	MouseDown,
};
#endif

#pragma endregion

#pragma region Structs

#ifndef Point
// ����� � ������������ X � Y
struct Point
{
	double X, Y;

	Point(double num = 0)
	{
		X = Y = num;
	}
	Point(double x, double y) 
	{
		X = x;
		Y = y;
	}
};
#endif 

#ifndef Color
// ���� � ������� RGB
struct Color
{
	int R, G, B;

	Color(int code = 0)
	{
		R = G = B = code;
	}
	Color(int r, int g, int b) 
	{
		R = r;
		G = g;
		B = b;
	}
};
#endif

#ifndef Brush
// �����
struct Brush
{
	double Size;
	Color BColor;
	Brush_Type Type;

	Brush(double size, Color color = Color{}, Brush_Type type = Brush_Type::Circle)
	{
		Size = size;
		BColor = color;
		Type = type;
	}
};
#endif

#ifndef Button
// ������ ����������
struct Button 
{
public:
	char* Caption;
	Point Position;
	Point Size;
	Color BaseColor;
	Color HowerColor;
	Color SelectedColor;
	ButtonStatus Status;

	Button(const char* caption = "", const Point& pos = Point{}, const Point& size = Point{},
		const Color& base = Color{}, const Color& select = Color{}, const Color& hower = Color{},
		const ButtonStatus& status = ButtonStatus::Unselected)
	{
		int len = strlen(caption);
		Caption = new char[len];
		for (int i = 0; i < len; i++)
			Caption[i] = caption[i];
		Position = pos;
		Size = size;
		BaseColor = base;
		SelectedColor = select;
		Status = status;
		HowerColor = hower;
	}
};
#endif

#pragma endregion

#ifndef Engine
// ����������� ����� ���������
class Engine 
{
	// TODO: ������������ ������� �������� ���� � ������

	// ������������ ������ (�������)
	GLsizei MaxHeight = 2000;

	// ������ ������ (�������)
	GLsizei MaxWidth = 2000;

	// ������ ����
	double WinWidth = 0;

	// ������ ������
	double WinHeight = 0;


	// ��� ����!
	const double ZMax = 100;


	// ������� "�����"
	Brush CurentBrush = Brush{
		10,
		Color{ 0,0,0 },
		Brush_Type::Circle
	};;

	// ������ ������� Buttons
	int ButtonsCount = 0;

	// ������ ������������������ ������
	Button* Buttons;

	// ������� ������
	Button* PressedButton;

	// ������ ������� (�������)
	const int charSize = 3;


	// ������������ �����
	// TODO: ����� ����������� - GLUT_BITMAP_TIMES_ROMAN_10
	void render_string(double x, double y, const char* string, Color const& color);

	// ������������ �������� ����������
	void init_ui();

	// ��������� ��������� ����������
	void draw_ui();


	// ����� ������ � ������� �������
	// ���� - ������� ������ ������ ��� -1 (���� � ���)
	int try_get_button_index(Point p);

	// ������ ����������� �������������
	GLvoid fill_rect(Point pos, Point size, Color c);

	// TODO
	// ������ ����������� ������������� � ����������� ������
	GLvoid fill_rounded_rect(Point pos, Point size, Color c);

	// TODO
	// ������ ����������� ����
	GLvoid fill_circle(Point pos, Point size, Color c);

	// ������ ������������� �����
	GLvoid draw_rect(Point pos, Point size, Color c);

	// ������ �����
	GLvoid draw_point(Point p);

public:

	// ��������� ����� ��� ������������ ������� ����
	// ���� - ��������� ��������� ����, ����������� ������/���������� ���� ��������� ��������
	GLvoid Resize(GLsizei width, GLsizei height);

	// �������������� ����� � ������� �������� �����
	GLvoid Init(GLvoid); 

	// ��������� (render) ����� 
	GLvoid Draw(GLvoid);

	// ������������� ��������� ��������� ��������������� OpenGL.
	int SetWindowPixelFormat(HDC hDC); 

	// ��������� ������� ���
	// ���� - ���������� ���� ����� ������������ � ��������������� ������� �������������
	GLvoid MouseDown(Point p, bool Shift);

	// ��������� ������� ���
	// ���� - ��������� ������� ���������� � ����������� ��������� (�� �������)
	GLvoid MouseUp(Point p, bool Shift);

	// ��������� ����������� ������� � ������� ���
	GLvoid MouseDrag(Point begin, Point end, bool Shift);

	// ��������� ����������� �������
	// ���� - �������� ������������ ��������� ��������� �������, ������������ ������ ��� �������� � �.�.
	GLvoid MouseHower(Point begin, Point end, bool Shift);
};
#endif


