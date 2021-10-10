#include "engine.h"
#include "App.h"
#include "api.h"
#include <cmath>

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

Point GetVectorProduct(Point p, double number)
{
	return Point{ p.x * number, p.y * number };
}

Point GetVectorsDifference(Point p1, Point p2)
{
	return Point{ p1.x - p2.x, p1.y - p2.y };
}

Point GetVectorsSumm(Point p1, Point p2)
{
	return Point{ p1.x + p2.x, p1.y + p2.y };
}

double GetVectorLen(Point p)
{
	return sqrt((p.x * p.x + p.y * p.y));
}

Point GetNormalizedVector(Point p)
{
	double len = GetVectorLen(p);
	Point result = Point{ p.x / len, p.y / len };
	return result;
}



#pragma region Source
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
	glOrtho(-Width, Width, -Height, Height, 0, ZMax * 2);

	// Действия будут производиться с матрицей модели
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Распологаем "камеру"
	GLdouble x0, y0, z0;
	x0 = Width - (GLdouble)1;
	y0 = 0 - (GLdouble)1;
	z0 = -ZMax;
	gluLookAt(x0, y0, z0, x0, y0, 0, 0, -1, 0);

	VidgetsCount = 3;
	Vidgets = new Vidget[3]{
		Vidget(Point{100, 50}, Point{300, 300}, Color{255,220,220}, Color{200,0,0}),
		Vidget(Point{500, 150}, Point{250, 250}, Color{220,255,220}, Color{0,200,0}),
		Vidget(Point{280, 480}, Point{200, 200}, Color{220,220,255}, Color{0,0,200}),
	};
}

// код функции, которая устанавливает параметры контекста воспроизведения OpenGL.
int Engine::SetWindowPixelFormat(HDC hDC)
{
	int m_GLPixelIndex;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
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
	// Передается на рассмотрение системе, выбранный нами формат пикселей. Функция просматривает в контексте устройства - hdc наиболее подходящий формат пикселей и выбирает его
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pfd);
	if (m_GLPixelIndex == 0) // Let's choose a default index.
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

	GLint x = 0;
	GLint y = height-Height;

	glViewport(x, y, Width*2, Height*2); // Устанавливается область просмотра

	glDepthRange(0, 1); //Глубина каждой точки записывается в z-буфер
	//с помощью которого OpenGL удаляет невидимые линии и поверхности
}

GLvoid Engine::Draw(GLvoid)
{
	// Очищается буфер кадра и буфер глубины
	// Текущая матрица сбрасывается на единичную
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < VidgetsCount; i++) {
		Vidget vidget = Vidgets[i];
		Color c = CurentVidgetID == i ? vidget.SelectedColor : vidget.BaseColor;
		draw_rect(vidget.Position, vidget.Size, c);
	}

	SwapBuffers(wglGetCurrentDC());
}


GLvoid Engine::MouseDown(Point p, bool Shift)
{
	CurentVidgetID = GetClickedVidgetId(p.x, p.y, Shift);
	if (CurentVidgetID == -1)
		if (Shift && LastPoint.x != -1)
		{
			DrawLine(LastPoint.x, LastPoint.y, p.x, p.y);
			LastPoint = p;
		}
		else {
			draw_point(p);
			LastPoint = p;
		}
}

GLvoid Engine::MouseDrag(Point begin, Point end)
{

}


GLvoid Engine::draw_rect(Point pos, Point size, Color c)
{
	glColor3ub(c.R, c.G, c.B);
	int x1 = pos.x;
	int x2 = pos.x + size.x;
	int y1 = pos.y;
	int y2 = pos.y + size.y;

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(x1, y1, 0);
	glVertex3f(x1, y2, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x2, y1, 0);	
	glVertex3f(x1, y1, 0);
	glEnd(); 
	glPopMatrix();
}

GLvoid Engine::draw_point(Point p)
{
	double R = CurentBrush.Size/2;
	int sigments_count = R * 8;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < sigments_count; i++)
	{
		float theta = 2 * 3.1415926f * i / sigments_count;
		float x = R * cosf(theta);
		float y = R * sinf(theta);

		glVertex2f(x + p.x, y + p.y);
	}
	glEnd();
}