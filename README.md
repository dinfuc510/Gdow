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
    config.borderRadius = 8;
    config.borderWidth = 1;
  
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
```
# Compiling

```
cl main.cpp /link user32.lib gdi32.lib gdiplus.lib
```
