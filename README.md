# Usage

```c++
#include "gdow.h"

#if !defined(_DEBUG) && defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif // _DEBUG
{
	Config config{};
	InitWindow(700, 500, L"", config);

	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
```
# Compiling

```
cl main.cpp /link user32.lib gdi32.lib gdiplus.lib
```
