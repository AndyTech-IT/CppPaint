//
// ������������ ����, �������� ��� �������� ������
// ���������� ������� ���������� ��� � engine.cpp
//

#include "api.h"
#include "Resource.h"

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

#ifndef UIButton
enum class UIButton {
	ChaingeColor,
	ChaingeSize,
	ChaingeType,
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
typedef struct Color
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
}Color;
#endif

#ifndef Brush
// �����
typedef struct Brush
{
	int Size;
	Color BColor;
	Brush_Type Type;

	Brush(int size, Color color = Color{}, Brush_Type type = Brush_Type::Circle)
	{
		Size = size;
		BColor = color;
		Type = type;
	}
}Brush;
#endif

#ifndef Button
// ������ ����������
struct Button 
{
public:
	// ������� ������
	char* Caption;
	// ������� ������ �������� ���� ������
	Point Position;
	// ������ ������
	Point Size;
	// �������� ����
	Color BaseColor;
	// ���� ��� ��������� �������
	Color HowerColor;
	// ���� ��� ������� �� ������
	Color SelectedColor;
	// ������� ��������� ������ 
	ButtonStatus Status;

	// �����������
	Button(const char caption[] = "", const Point& pos = Point{}, const Point& size = Point{},
		const Color& base = Color{}, const Color& select = Color{}, const Color& hower = Color{},
		const ButtonStatus& status = ButtonStatus::Unselected)
	{
		int len = strlen(caption);
		Caption = new char[len+1] {};
		for (int i = 0; i < len; i++)
			Caption[i] = caption[i];
		Caption[len] = '\0';
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
	// ���������� ������� �� ����� �� ������
	void OnButtonClick(Button& sender, int sender_id);
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

	// �������������� ����
	void init_canvas(Point size);
	// ������� ����� ����� ������
	void clear_canvas();
	// ������ ����� �� ������
	void draw_canvas(); 
	// ���������� ������ "�������" �� ������� ����� ������������
	Point GetCanvasPoint(Point p);

	// ����
	HINSTANCE hInst; 
	// ����
	HWND hWnd;
	
	// �����
	Color** Canvas;
	// ������ ������
	Point CanvasSize = Point(0);
	// ������� ������ �������� ���� ������
	Point CanvasPos = Point(5, 120);

	// ������ ������ ������� �� ������
	double PixelSize = 10.0;

public:

	// ��������� ����� ��� ������������ ������� ����
	// ���� - ��������� ��������� ����, ����������� ������/���������� ���� ��������� ��������
	GLvoid Resize(GLsizei width, GLsizei height);

	// �������������� ����� � ������� �������� �����
	GLvoid Init(HINSTANCE hInst, HWND hWnd);

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

// ������� "�����"
static Brush CurentBrush = Brush(10, Color(0, 0, 0));
// ���� �� ������ ��������� ��� ���������� �������... ��� ����� ������� �� ����������� �� ����������, � ������ ������ ��� ��� ���������.