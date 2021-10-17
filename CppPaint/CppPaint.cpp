#include "CppPaint.h"
#define MAX_LOADSTRING 100

#pragma region INIT
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

double WinHeinght = 800;
double WinWidth = 800;

HWND hWnd;
HGLRC hGLRC;
HDC hDC;
Point mouse_point;
Engine* engine;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPPAINT));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPPAINT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CPPPAINT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_CPPPAINT));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WinWidth, WinHeinght, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}
#pragma endregion

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // Создание главного окна
    case WM_CREATE:
    {
        engine = new Engine();
        hDC = GetDC(hWnd); // получаем контекст устройства нашего окна
        engine->SetWindowPixelFormat(hDC); // устанавливаем параметры контекста воспроизведения OpenGL
        hGLRC = wglCreateContext(hDC); // создаем контекст воспроизведения OpenGL
        wglMakeCurrent(hDC, hGLRC); // делаем его текущим
        engine->Init(hInst, hWnd);
    }
    break;

    // Обработка меню приложения
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    // Отрисовка главного окна
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        engine->Draw();
        EndPaint(hWnd, &ps);
    }
    break;

    // Обработка изменения размера главного окна
    case WM_SIZE:
    {
        engine->Resize(LOWORD(lParam), HIWORD(lParam));
    }
    break;

    // Удаление фона...
    case WM_ERASEBKGND:
    {
        return 1;
    }
    break;

    // Нажатие ЛКМ
    case WM_LBUTTONDOWN:
    {
        mouse_point.X = LOWORD(lParam); // Координаты в системе окна
        mouse_point.Y = HIWORD(lParam);

        engine->MouseDown(mouse_point, GetKeyState(VK_SHIFT) < 0);

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    // Отпуск ЛКМ
    case WM_LBUTTONUP:
    {
        mouse_point.X = LOWORD(lParam); // Координаты в системе окна
        mouse_point.Y = HIWORD(lParam);

        engine->MouseUp(mouse_point, GetKeyState(VK_SHIFT) < 0);

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    // Перемещение курсора
    case WM_MOUSEMOVE:
    {
        Point begin = mouse_point;
        mouse_point.X = LOWORD(lParam); // Координаты в системе окна
        mouse_point.Y = HIWORD(lParam);
        if (UINT(wParam) & MK_LBUTTON)
        {
            engine->MouseDrag(begin, mouse_point, GetKeyState(VK_SHIFT) < 0);

        }
        else {
            engine->MouseHower(begin, mouse_point, GetKeyState(VK_SHIFT) < 0);
        }
        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    // Закрытие окна
    case WM_DESTROY:
    {
        if (hGLRC)
        { // удаляем созданный выше контекст воспроизведения OpenGL
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hGLRC);
        } // освобождаем контекст устройства нашего окна
        ReleaseDC(hWnd, hDC);

        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//
//  ФУНКЦИЯ: About(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в диалоговом окне About.
//
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    // Инициализация диалога
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    // Обработка меню диалога
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
