#undef UNICODE
#include <windows.h>
#include  <math.h>
#include <cmath>
using namespace std;

BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void DrawEllipse(HDC& hdc, int x, int y);
void DrawRectangle(HDC& hdc, int x, int y);
void DrawSector(HDC& hdc, int x, int y);
void DrawRhombus(HDC& hdc, int x, int y);

static char appName[] = "Lab4";
static char title[] = "Title";

HINSTANCE h;
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
		DrawEllipse(hdc, x, y);

		// Drawing rectangle
		DrawRectangle(hdc, x, y);
		// Pie
		DrawSector(hdc, x, y);

		DrawRhombus(hdc, x, y);
		// Rhomb

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

void DrawEllipse(HDC& hdc, int x, int y)
{
	MoveToEx(hdc, 0, y / 2, NULL);
	LineTo(hdc, x, y / 2);
	MoveToEx(hdc, x / 2, 0, NULL);
	LineTo(hdc, x / 2, y);
	HPEN hpen, lastPen; hpen = CreatePen(PS_DASHDOTDOT, 2, RGB(122, 0, 0));
	lastPen = (HPEN)SelectObject(hdc, hpen);

	// OR GetStockIbject(DC_PEN), seydccolor ...
	// Drawing ellipse 
	Ellipse(hdc, 0, 0, x / 2, y / 2 - 0.1*y);

	RECT textout;
	textout.left = 0; textout.top = y / 2 - 0.1*y; textout.right = x / 2; textout.bottom = y / 2;
	DrawText(hdc, "Ellipse", 8, &textout, DT_CENTER | DT_VCENTER);

	SelectObject(hdc, lastPen);
	DeleteObject(hpen);
}

void DrawRectangle(HDC& hdc, int x, int y)
{

	RECT textout;
	HBRUSH brush, oldB;

	LOGBRUSH lb;
	lb.lbColor = RGB(241, 12, 111);
	lb.lbHatch = HS_VERTICAL;
	lb.lbStyle = BS_HATCHED;
	brush = CreateBrushIndirect(&lb);
	oldB = (HBRUSH)SelectObject(hdc, brush);

	//lastPen = (HPEN)SelectObject(hdc, hpen);
	RECT r;
	r.left = x / 2 + 0.02*x;
	r.top = 0;
	r.right = x - 0.02*x;
	r.bottom = y / 2 - y*0.1;
	FillRect(hdc, &r, brush);
	Rectangle(hdc, x / 2 + 0.02*x, 0, x - 0.02*x, y / 2 - y*0.1);
	textout.left = x / 2; textout.top = y / 2 - 0.05*y; textout.right = x; textout.bottom = y / 2;

	SelectObject(hdc, oldB);
	DeleteObject(brush);

}

void DrawSector(HDC& hdc, int x, int y)
{
	RECT textout;

	HBRUSH  brush = CreateHatchBrush(HS_CROSS, RGB(10, 10, 150));
	HBRUSH lastBrush = (HBRUSH)SelectObject(hdc, brush);
	int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, R;
	if (y <= x)
	{
		R = y / 4;
		x1 = ((x / 2 - y / 2) / 2); y1 = y / 2;
		x2 = x / 2 - x1; y2 = y;
	}
	else
	{
		R = x / 4;
		x1 = 0; y1 = y / 2 + ((y / 2 - x / 2) / 2);
		x2 = x / 2; y2 = y - ((y / 2 - x / 2) / 2);
	}

	x0 = x / 4; y0 = y*0.75;
	x3 = x0 + R; y3 = y0;
	x4 = x0 + R * cos(3.14 / 6);
	y4 = y0 - R * sin(3.14 / 6);
	Pie(hdc, x1, y1, x2, y2, x3, y3, x4, y4);

	textout.left = 0; textout.top = y - 0.05*y; textout.right = x / 2; textout.bottom = y;
	DrawText(hdc, "Sector", 6, &textout, DT_CENTER | DT_BOTTOM);

	SelectObject(hdc, lastBrush);
	DeleteObject(brush);
}

void DrawRhombus(HDC& hdc, int x, int y)
{
	HPEN newPen, oldPen;
	HBRUSH newBrush, oldBrush;

	newBrush = CreateSolidBrush(RGB(255, 0, 0));
	newPen = CreatePen(PS_ALTERNATE, 5, RGB(100, 54, 78));
	oldPen = (HPEN)SelectObject(hdc, newPen);
	oldBrush = (HBRUSH)SelectObject(hdc, newBrush);

	RECT textout;
	POINT points[4];
	points[0].x = x * 0.75; points[0].y = y / 2 + 0.05*y;
	points[1].x = x - 0.03*x; points[1].y = y * 0.75;
	points[2].x = x *0.75; points[2].y = y - 0.05*y;
	points[3].x = x / 2 + 0.03*x; points[3].y = y*0.75;

	textout.left = x / 2; textout.top = y - 0.05*y; textout.right = x; textout.bottom = y;
	DrawText(hdc, "Rhomb", 5, &textout, DT_CENTER | DT_BOTTOM);

	Polygon(hdc, points, 4);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(newPen);
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