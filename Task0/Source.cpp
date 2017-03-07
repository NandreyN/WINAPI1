#undef UNICODE
#
#include <windows.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	char* data; data = lpCmdLine;
	for (int i =0;i < strlen(data); i++)
		if(!isdigit(data[i]))
		{
			MessageBox(NULL, "Error", "Error", MB_OK);
			return TRUE;
		}
	MessageBox(NULL, data, "Info", MB_OK);
	return FALSE;
}