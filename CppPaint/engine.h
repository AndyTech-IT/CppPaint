//
// Заголовочный файл, хранящий все элементы движка
// Реализация которых происходит уже в engine.cpp
//

#include "api.h"

#pragma region Enums

#ifndef Brush_Type
// Тип кисти
enum class Brush_Type
{
	Square,
	Circle,
};
#endif

#ifndef ButtonStatus
// Статус кнопки
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
// Точка с компанентами X и Y
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
// Цвет в формате RGB
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
// Кисть
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
// Кнопка интерфейса
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
// Графический движёк программы
class Engine 
{
	// TODO: Переработать систему размеров окна и холста

	// Максимальная высота (пиксели)
	GLsizei MaxHeight = 2000;

	// Ширина экрана (пиксели)
	GLsizei MaxWidth = 2000;

	// Ширина окна
	double WinWidth = 0;

	// Высота экрана
	double WinHeight = 0;


	// Так надо!
	const double ZMax = 100;


	// Текущая "Кисть"
	Brush CurentBrush = Brush{
		10,
		Color{ 0,0,0 },
		Brush_Type::Circle
	};;

	// Размер массива Buttons
	int ButtonsCount = 0;

	// Массив зарегистрированных кнопок
	Button* Buttons;

	// Текушая кнопка
	Button* PressedButton;

	// Размер символа (пиксели)
	const int charSize = 3;


	// Отрисовывает текст
	// TODO: Шрифт захардкожен - GLUT_BITMAP_TIMES_ROMAN_10
	void render_string(double x, double y, const char* string, Color const& color);

	// Регистрирует элементы интерфейса
	void init_ui();

	// Отрисовка элементов интерфейса
	void draw_ui();


	// Поиск кнопки в текущей позиции
	// Цель - вернуть индекс кнопки или -1 (если её нет)
	int try_get_button_index(Point p);

	// Рисует закрашенный прямоугольник
	GLvoid fill_rect(Point pos, Point size, Color c);

	// TODO
	// Рисует закрашенный прямоугольник с скруглёнными углами
	GLvoid fill_rounded_rect(Point pos, Point size, Color c);

	// TODO
	// Рисует закрашенный круг
	GLvoid fill_circle(Point pos, Point size, Color c);

	// Рисует прямоугольную рамку
	GLvoid draw_rect(Point pos, Point size, Color c);

	// Рисует точку
	GLvoid draw_point(Point p);

public:

	// Подгоняет движёк под изменившиеся размеры окна
	// Цель - сохранить пропорции окна, компенсируя сжатие/растяжение окна смещением картинки
	GLvoid Resize(GLsizei width, GLsizei height);

	// Подготавливает движёк к первому рабочему циклу
	GLvoid Init(GLvoid); 

	// Отрисовка (render) сцены 
	GLvoid Draw(GLvoid);

	// Устанавливает параметры контекста воспроизведения OpenGL.
	int SetWindowPixelFormat(HDC hDC); 

	// Обработка нажатие ЛКМ
	// Цель - определить куда нажал пользователь и соответствующим образом отриагировать
	GLvoid MouseDown(Point p, bool Shift);

	// Обработка отпуска ЛКМ
	// Цель - Выполнить возврат интерфейса в изначальное состояние (до нажатия)
	GLvoid MouseUp(Point p, bool Shift);

	// Обработка перемещения курсора с зажатой ЛКМ
	GLvoid MouseDrag(Point begin, Point end, bool Shift);

	// Обработка перемещения курсора
	// Цель - показать пользователю возможный результат нажатия, подсвечивать кнопки под курсором и т.п.
	GLvoid MouseHower(Point begin, Point end, bool Shift);
};
#endif


