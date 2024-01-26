#ifndef TITLEBAR_ACTIVE_COLOR
#define TITLEBAR_ACTIVE_COLOR 0xff2f2f2f
#endif //!TITLEBAR_ACTIVE_COLOR
#ifndef TITLEBAR_INACTIVE_COLOR
#define TITLEBAR_INACTIVE_COLOR 0xff3e3e3e
#endif //!TITLEBAR_INACTIVE_COLOR

#define BUTTON_ICON_MOUSELEFT_COLOR 0xffffffff

#include "../gdow.h"

#if !defined(_DEBUG) && defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif // _DEBUG
{
	Config config{};
	config.borderRadius = 8;
	config.borderWidth = 1;
	config.borderColor = 0xaa7f7f7f;
	config.background = 0xff1e1e1e;

	HWND hwnd = InitWindow(700, 500, L"", config);
	if (hwnd == nullptr) {
		return -1;
	}
	HWND baseWindow = FindWindowEx(GetParent(hwnd), nullptr, L"BaseWindow", nullptr);
	if (baseWindow == nullptr) {
		return -1;
	}

	HWND active = GetActiveWindow();
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		HWND temp = GetActiveWindow();
		if ((active == nullptr) ^ (temp == nullptr)) {
			active = temp;
			DrawBaseWindow(baseWindow, active != nullptr);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
