#undef UNICODE
#include <windows.h>
#include  <math.h>
#include <cmath>
using namespace std;

BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
static char appName[] = "Lab4";
static char title[] = "Title";


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitApplication(hinstance))
	{
		MessageBox(NULL, "Unable to Init App", "Error", MB_OK);
		return FALSE;
	}

	if (!InitInstance(hinstance, nCmdShow))
	{
		MessageBox(NULL, "Unable to Init Instance", "Error", MB_OK);
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL InitApplication(HINSTANCE hinstance)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = appName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, "Cannot register class", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;
} 
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int x, y;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT clientRect;

	switch (message)
	{
	case WM_SIZE:
		x = LOWORD(lparam);
		y = HIWORD(lparam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		HGDIOBJ lastPen, lastBrush;
		MoveToEx(hdc, 0, y / 2, NULL);
		LineTo(hdc, x, y / 2);
		MoveToEx(hdc, x / 2, 0, NULL);
		LineTo(hdc, x / 2, y);
		HPEN hpen; hpen = CreatePen(PS_DASHDOTDOT, 2, RGB(122, 0, 0));
		lastPen = SelectObject(hdc, hpen);

		// OR GetStockIbject(DC_PEN), seydccolor ...
		// Drawing ellipse 
		Ellipse(hdc, 0, 0, x / 2, y / 2 - 0.1*y);

		HFONT hfont; hfont =  (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONT logfont;
		GetObject(hfont, sizeof(LOGFONT), &logfont);
		logfont.lfHeight = 0.05*y;
		HFONT newFont; newFont = CreateFontIndirect(&logfont);
		HFONT oldFont; oldFont =  (HFONT)SelectObject(hdc, newFont);

		TextOut(hdc, x / 4.5, y / 2 - 0.1*y, "Ellipse", 8);
		// Drawing rectangle
		hpen = CreatePen(PS_ALTERNATE, 3, RGB(50, 210, 200));
		SelectObject(hdc, hpen);
		Rectangle(hdc, x / 2 + x*0.02, 0 + y*0.06, x - x*0.02, y / 2 - y*0.06); // использовать единичную окружность!!!!!!!
		TextOut(hdc, x*0.75, y / 2 - y*0.05, "Rectangle", 9);
		SelectObject(hdc, lastPen);
		DeleteObject(hpen);
		// Pie

		HBRUSH brush; brush = CreateSolidBrush(RGB(10, 10, 150));
		lastBrush = SelectObject(hdc, brush);
		int x0, y0, x1, y1, x2, y2, R;
		if (y <= x)
			R = y / 2 / 2;
		else
			R = x / 2 / 2;

		x0 = R;
		y0 = y / 2 + R;
		x1 = 2 * R;
		y1 = y / 2 + R;
		x2 = x0 + R*cos(3.14 / 2);
		y2 = y0 - R*sin(3.14 / 2);
		Pie(hdc, 0, y / 2, 2 * R, y / 2 + 2 * R, x1, y1, x2, y2);
		TextOut(hdc, x*0.2, y - 0.05*y, "Sector", 6);
		SelectObject(hdc, lastBrush);
		DeleteObject(brush);

		// Rhomb
		POINT points[4];
		points[0].x = x * 0.75; points[0].y = y / 2 + 0.05*y;
		points[1].x = x - 0.03*x; points[1].y = y * 0.75;
		points[2].x = x *0.75; points[2].y = y - 0.05*y;
		points[3].x = x / 2 + 0.03*x; points[3].y = y*0.75;
		TextOut(hdc, 0.75*x, y - 0.05*y, "Rhomb", 5);

		SelectObject(hdc, oldFont);
		DeleteObject(oldFont);
		Polygon(hdc, points, 4);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return FALSE;
}
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	hwnd = CreateWindow(
		appName,         // указатель на строку зарегистрированного имени класса
		title,             // указатель на строку заголовка окна
		WS_OVERLAPPEDWINDOW, // стиль окна
		CW_USEDEFAULT,       // горизонтальная координата окна
		0,       // вертикальная координата окна
		CW_USEDEFAULT,       // ширина окна
		0,       // высота окна
		NULL,                // дескриптор родительского окна
		NULL,                // дескриптор меню окна
		hinstance,           // дескриптор экземпляра приложения
		NULL);


	if (!hwnd)
		return FALSE;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}