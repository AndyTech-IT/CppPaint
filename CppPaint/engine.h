//
// Заголовочный файл, хранящий все элементы движка
// Реализация которых происходит уже в engine.cpp
//

#include "api.h"
#include "Resource.h"

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
// Кисть
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
// Кнопка интерфейса
struct Button 
{
public:
	// Подпись кнопки
	char* Caption;
	// Позиция левого верхнего угла кнопки
	Point Position;
	// Размер кнопки
	Point Size;
	// Основной цвет
	Color BaseColor;
	// Цвет при наведении курсора
	Color HowerColor;
	// Цвет при нажатии на кнопку
	Color SelectedColor;
	// Текущее состояние кнопки 
	ButtonStatus Status;

	// Конструктор
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
	// Обработчик нажатия на любую из кнопок
	void OnButtonClick(Button& sender, int sender_id);
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

	// Инициализирует холс
	void init_canvas(Point size);
	// Очищает холст белым цветом
	void clear_canvas();
	// Рисует холст на экране
	void draw_canvas(); 
	// Возвращает индекс "пикселя" на который нажал пользователь
	Point GetCanvasPoint(Point p);

	// Надо
	HINSTANCE hInst; 
	// Надо
	HWND hWnd;
	
	// Холст
	Color** Canvas;
	// Размер холста
	Point CanvasSize = Point(0);
	// Позиция левого верхнего угла холста
	Point CanvasPos = Point(5, 120);

	// Размер одного пикселя на холсте
	double PixelSize = 10.0;

public:

	// Подгоняет движёк под изменившиеся размеры окна
	// Цель - сохранить пропорции окна, компенсируя сжатие/растяжение окна смещением картинки
	GLvoid Resize(GLsizei width, GLsizei height);

	// Подготавливает движёк к первому рабочему циклу
	GLvoid Init(HINSTANCE hInst, HWND hWnd);

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

// Текущая "Кисть"
static Brush CurentBrush = Brush(10, Color(0, 0, 0));
// Выше вы можете наблюдать так называемый костыль... Это когда сделать по нормальному не получилось, а значит делаем так как получится.