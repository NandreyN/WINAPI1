#include <windows.h>
#include <ctime>


BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCMdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT messagem, WPARAM wparam, LPARAM lparam);
void Paint(HWND hwnd, int x, int y);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));
	MSG msg;
	InitApplication(hInstance);
	InitInstance(hInstance, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int x, y;
	switch (message)
	{
	case WM_SIZE:
		x = LOWORD(lparam);
		y = HIWORD(lparam);
		break;
	case WM_PAINT:
		Paint(hwnd, x, y);
		break;
	case  WM_CLOSE:
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
BOOL InitApplication(HINSTANCE hinstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = (LPSTR)"Rectangles";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszMenuName = NULL;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Unable to init app", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;
}
BOOL InitInstance(HINSTANCE hinstance, int nCMdShow)
{
	HWND hwnd; hwnd = CreateWindow(
		"Rectangles",
		"Rectangles",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hinstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, "Cannot init instance", "Error", MB_OK);
		return FALSE;
	}

	ShowWindow(hwnd, nCMdShow);
	UpdateWindow(hwnd);
	return TRUE;
}
void Paint(HWND hwnd, int x, int y)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rect);
	HBRUSH brush;
	int count, cx, cy, clStep1, clStep2, clStep3;
	count = 20;
	cx = x / count;
	cy = y / count;
	

	/* generate secret number between 1 and 10: */
	int r0 = rand() % 255 + 1;
	int g0 = rand() % 255 + 1;
	int b0 = rand() % 255 + 1;

	clStep1 = (255 - r0) / count;
	clStep2 = (255 - g0) / count;
	clStep3 = (255 - b0) / count;
	for (int i = 0; i < count && rect.left <= x / 2 && rect.top <= y / 2; i++)
	{
		rect.left += cx;
		rect.top += cy;

		rect.right -= cx;
		rect.bottom -= cy;

		brush = CreateSolidBrush(RGB(r0 + clStep1*i, g0 + clStep2*i, b0 + clStep3*i));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}


	DeleteObject(brush);
	EndPaint(hwnd, &ps);
}