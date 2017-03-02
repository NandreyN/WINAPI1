#include <windows.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
struct Day
{
	std::string dayName;
	std::vector<std::string> classes;
};
ostream& operator<< (ostream& o, Day& d)
{
	return o;
}

istream& operator >> (istream& i, Day& d)
{
	d.dayName = "";
	d.classes.clear();
	string data; getline(i, data);
	istringstream iss(data);
	iss >> d.dayName;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(d.classes));
	return i;
}
BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCMdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT messagem, WPARAM wparam, LPARAM lparam);
void InitCollection(std::vector<Day>&);
void DrawDay(HDC& hdc, Day& day, POINT p1, POINT p2);
void DrawTimeTable(HWND& hwnd, vector<Day>& days, int x, int y);
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
	static vector<Day> week;

	switch (message)
	{
	case WM_CREATE:
		InitCollection(week);
		break;
	case WM_SIZE:
		x = LOWORD(lparam);
		y = HIWORD(lparam);
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		RECT r;
		GetClientRect(hwnd, &r);

		for (int y1 = 0, i = 0; i < week.size(); y1 += y / week.size(), i++)
		{
			r.left = 0; // Дебилизм
			r.top = y1;
			r.right = x;
			r.bottom = y1 + y / week.size();
			POINT p1, p2;
			p1.x = r.left; p1.y = r.top; p2.x = r.right; p2.y = r.bottom;
			DrawDay(hdc, week[i], p1, p2);
		}
		EndPaint(hwnd, &ps);
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
	wc.lpszClassName = (LPSTR)"Timetable";
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
		"Timetable",
		"Timetable",
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
void InitCollection(std::vector<Day>& coll)
{
	ifstream in("in.txt");
	copy(istream_iterator<Day>(in), istream_iterator<Day>(), back_inserter(coll));
	in.close();
}
void DrawDay(HDC& hdc, Day& day, POINT p1, POINT p2)
{
	HFONT hfont; hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT turnedTextLF;
	turnedTextLF.lfCharSet = DEFAULT_CHARSET;
	turnedTextLF.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(turnedTextLF.lfFaceName, "Arial");
	turnedTextLF.lfHeight = (6 * (p2.y - p1.y) < p2.x - p1.x) ? (p2.y - p1.y) / 4 : (p2.x - p1.x) / 28;
	turnedTextLF.lfWidth = turnedTextLF.lfHeight *0.4;
	turnedTextLF.lfEscapement = 450;
	turnedTextLF.lfItalic = FALSE;
	turnedTextLF.lfUnderline = FALSE;
	turnedTextLF.lfStrikeOut = FALSE;
	SetBkMode(hdc, TRANSPARENT);

	HFONT hTurnedText = CreateFontIndirect(&turnedTextLF);
	HFONT oldFont = (HFONT)SelectObject(hdc, hTurnedText);

	HPEN hNewPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hNewPen);

	int xHorizontalCellLen;
	int delta;
	delta = (p1.x == 0) ? p2.x * 0.1 : p2.x - p1.x;
	xHorizontalCellLen = (p2.x - p1.x - delta) / 5;

	RECT twR;
	twR.left = 0;
	twR.top = p1.y;
	twR.right = delta;
	twR.bottom = p2.y;

	DrawText(hdc, day.dayName.c_str(), strlen(day.dayName.c_str()),&twR, DT_BOTTOM | DT_INTERNAL | DT_LEFT| DT_SINGLELINE);

	SelectObject(hdc, oldFont);
	DeleteObject(hTurnedText);
	for (int i = p1.x + delta, j = 0; i <= p2.x - p1.x; i += xHorizontalCellLen, j++)
	{
		RECT r;
		r.left = i;
		r.top = p1.y;
		r.right = i + xHorizontalCellLen;
		r.bottom = p2.y;
		FillRect(hdc, &r, CreateSolidBrush(RGB(180, 180, 180)));
		if (j < day.classes.size())
		{
			DrawText(hdc, day.classes[j].c_str(), strlen(day.classes[j].c_str()), &r, DT_CENTER |DT_INTERNAL | DT_VCENTER | DT_SINGLELINE);
		}
	}
	for (int i = p1.x + delta; i <= p2.x - p1.x; i += xHorizontalCellLen)
	{
		MoveToEx(hdc, i, p1.y, NULL);
		LineTo(hdc, i, p2.y + (p2.y - p1.y));
	}
	MoveToEx(hdc, p1.x, p2.y, NULL);
	LineTo(hdc, p2.x, p2.y);


	SelectObject(hdc, hOldPen);
	DeleteObject(hOldPen);
}