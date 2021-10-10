//
// Файл хранящий исходный код движка
// Реализует все объявленные в engine.h функции
//

#include "api.h"

#include "engine.h"
#include <cmath>

#pragma region  Additional func
// Расширяет массив, дописывая новый элемент в его конец
template <typename T>
void UppendArray(T*& arr, T item, int& size)
{
	T* result = new T[size + 1]{};
	for (int i = 0; i < size; i++)
		result[i] = arr[i];
	result[size] = item;
	size++;
	delete[] arr;
	arr = result;
}

// Выполняет умножение компонентов вектора на число
Point GetVectorProduct(Point p, double number)
{
	return Point{ p.X * number, p.Y * number };
}

// Векторно вычитает из р1 - р2
Point GetVectorsDifference(Point p1, Point p2)
{
	return Point{ p1.X - p2.X, p1.Y - p2.Y };
}

// Векторно складывает р1 и р2
Point GetVectorsSumm(Point p1, Point p2)
{
	return Point{ p1.X + p2.X, p1.Y + p2.Y };
}

// Вычисляет длинну вектора
double GetVectorLen(Point p)
{
	return sqrt((p.X * p.X + p.Y * p.Y));
}

// Выполняет нормализацию вектора
Point GetNormalizedVector(Point p)
{
	double len = GetVectorLen(p);
	Point result = Point{ p.X / len, p.Y / len };
	return result;
}
#pragma endregion

#pragma region Init
GLvoid Engine::Init(GLvoid) 
{
	// Устанавливается синий фон
	glClearColor(1, 1, 1.0f, 1.0f); 

	// Устанавливается значение для
	// заполнения буфера глубины по умолчанию
	glClearDepth(1.0f); 

	// Включается тест глубины
	glEnable(GL_DEPTH_TEST); 

	// Устанавливается значение, используемое
	// в сравнениях при использовании
	// буфера глубины
	glDepthFunc(GL_LEQUAL); 

	// Включается плавное затенение
	glShadeModel(GL_SMOOTH); 

	// Включается сглаживание линий
	//glEnable(GL_LINE_SMOOTH); 
	

	// Выбирается самый качественный
	// режим сглаживания для линий
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	
	// Включается смешение цветов, необходимое
	glEnable(GL_BLEND); 

	// для работы сглаживания и задается
	// способ смешения
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	
	// Действия будут производиться с матрицей проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливается ортогональная проекция 
	glOrtho(-MaxWidth, MaxWidth, -MaxHeight, MaxHeight, 0, ZMax * 2);

	// Действия будут производиться с матрицей модели
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Распологаем "камеру"
	GLdouble x0, y0, z0;
	x0 = MaxWidth - (GLdouble)1;
	y0 = 0 - (GLdouble)1;
	z0 = -ZMax;
	gluLookAt(x0, y0, z0, x0, y0, 0, 0, -1, 0);

	// Регистрируем элементы интерфейса
	init_ui();
}

int Engine::SetWindowPixelFormat(HDC hDC)
{
	int m_GLPixelIndex;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	// Передается на рассмотрение системе, выбранный нами формат пикселей. 
	// Функция просматривает в контексте устройства - hdc наиболее подходящий формат пикселей и выбирает его
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pfd);
	if (m_GLPixelIndex == 0)
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
			return 0;
	}

	// установить формат пикселей в контексте устройства
	if (SetPixelFormat(hDC, m_GLPixelIndex, &pfd) == FALSE)
		return 0;
	return 1;
}
#pragma endregion

GLvoid Engine::Resize(GLsizei width, GLsizei height) {
	if (height == 0)
	{
		height = 1;
	}

	WinHeight = height;
	WinWidth = width;

	GLint X = 0;
	GLint Y = height-MaxHeight;

	glViewport(X, Y, MaxWidth*2, MaxHeight*2); // Устанавливается область просмотра

	glDepthRange(0, 1); //Глубина каждой точки записывается в z-буфер
	//с помощью которого OpenGL удаляет невидимые линии и поверхности
}

GLvoid Engine::Draw(GLvoid)
{
	// Очищается буфер кадра и буфер глубины
	// Текущая матрица сбрасывается на единичную
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_ui();

	// Отрисовка на экране данных из буфера... вроде...
	SwapBuffers(wglGetCurrentDC());
}

GLvoid Engine::MouseDown(Point p, bool Shift)
{
	int index = try_get_button_index(p); 
	if (index != -1)
	{
		Buttons[index].Status = ButtonStatus::MouseDown;
		PressedButton = &Buttons[index];
	}
	else
		(*PressedButton).Status = ButtonStatus::Unselected;
}

GLvoid Engine::MouseUp(Point p, bool Shift)
{
	int index = try_get_button_index(p);
	(*PressedButton).Status = ButtonStatus::Unselected;
	if (index != -1)
	{
		Buttons[index].Status = ButtonStatus::MouseHower;
	}
}

GLvoid Engine::MouseDrag(Point begin, Point end, bool shift)
{

}

GLvoid Engine::MouseHower(Point begin, Point end, bool Shift)
{
	int index = try_get_button_index(begin);
	if (index != -1)
	{
		Buttons[index].Status = ButtonStatus::Unselected;
	}

	index = try_get_button_index(end);
	if (index != -1)
	{
		Buttons[index].Status = ButtonStatus::MouseHower;
	}
}

GLvoid Engine::draw_rect(Point pos, Point size, Color c)
{
	
}

GLvoid Engine::draw_point(Point p)
{
	double R = CurentBrush.Size/2;
	int sigments_count = R * 8;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < sigments_count; i++)
	{
		float theta = 2 * 3.1415926f * i / sigments_count;
		float X = R * cosf(theta);
		float Y = R * sinf(theta);

		glVertex2f(X + p.X, Y + p.Y);
	}
	glEnd();
}

void Engine::init_ui() {
	ButtonsCount = 5;
	Buttons = new Button[ButtonsCount]{
		Button("Hello", Point(10), Point(100), Color(250), Color(240), Color(230)),
		Button("Hello", Point(110, 10), Point(100), Color(250), Color(240), Color(230)),
		Button("Hello", Point(220, 10), Point(100), Color(250), Color(240), Color(230)),
		Button("Hello", Point(330, 10), Point(100), Color(250), Color(240), Color(230)),
		Button("Hello", Point(440, 10), Point(100), Color(250), Color(240), Color(230)),
	};

	// Костыль, иначе при нажатии мимо будет исключение
	PressedButton = &Buttons[0];
}

void Engine::draw_ui() {
	// Отрисовка кнопок
	for (int i = 0; i < ButtonsCount; i++)
	{
		Button b = Buttons[i];

		// Выбор цвета кнопки исходя из её статуса
		Color c;
		switch (b.Status)
		{
		case ButtonStatus::MouseHower:
			c = b.HowerColor;
			break; 

		case ButtonStatus::MouseDown:
				c = b.SelectedColor;
				break;

		case ButtonStatus::Unselected:
		default:
			c = b.BaseColor;
			break;
		}

		// Отрисовка "тела" кнопки
		fill_rect(b.Position, b.Size, c);

		// Расчёт позиции кнопки поцентру и снизу
		double max_chars = b.Size.X / charSize;
		double caption_len = strlen(b.Caption);
		double text_pos_x = b.Position.X + (max_chars - caption_len) / 2 * charSize;
		double text_pos_y = b.Position.Y + b.Size.Y - charSize;

		// Отрисовка надписи кнопки
		render_string(text_pos_x, text_pos_y, b.Caption, Color());
	}
}

int Engine::try_get_button_index(Point p)
{
	for (int i = 0; i < ButtonsCount; i++)
	{
		Button b = Buttons[i];

		// Пограничные точки кнопки
		int min_x = b.Position.X;
		int max_x = b.Position.X + b.Size.X;
		int min_y = b.Position.Y;
		int max_y = b.Position.Y + b.Size.Y;

		if (min_x <= p.X && p.X <= max_x &&
			min_y <= p.Y && p.Y <= max_y)
			return i;
	}
	return -1;
}

GLvoid Engine::fill_rect(Point pos, Point size, Color c)
{
	glColor3ub(c.R, c.G, c.B);

	// Пограничные точки
	int min_x = pos.X;
	int max_x = pos.X + size.X;
	int min_y = pos.Y;
	int max_y = pos.Y + size.Y;

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(min_x, min_y, 0);
	glVertex3f(min_x, max_y, 0);
	glVertex3f(max_x, max_y, 0);
	glVertex3f(max_x, min_y, 0);
	glVertex3f(min_x, min_y, 0);
	glEnd();
	glPopMatrix();
}

GLvoid Engine::fill_rounded_rect(Point pos, Point size, Color c)
{
	return GLvoid();
}

GLvoid Engine::fill_circle(Point pos, Point size, Color c)
{
	return GLvoid();
}

void Engine::render_string(double X, double Y, const char* string, Color const& color)
{
	glColor3f(color.R, color.G, color.B);
	glRasterPos2f(X, Y);
	for (int i = 0; i < strlen(string); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, string[i]);
	}
	glPopMatrix();
}