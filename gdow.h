#ifndef UNICODE
#define UNICODE
#endif //!UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif //!_UNICODE

#ifndef GDOW_H
#define GDOW_H

#include <windows.h>
#include <gdiplus.h>
#include <memory>

#pragma comment(lib, "gdiplus.lib")

#ifndef TITLEBAR_HEIGHT
#define TITLEBAR_HEIGHT 32
#endif //!TITLEBAR_HEIGHT
#ifndef TITLEBAR_ACTIVE_COLOR
#define TITLEBAR_ACTIVE_COLOR 0xffa1a1a1
#endif //!TITLEBAR_ACTIVE_COLOR
#ifndef TITLEBAR_INACTIVE_COLOR
#define TITLEBAR_INACTIVE_COLOR 0xff787878
#endif //!TITLEBAR_INACTIVE_COLOR

#ifndef BUTTON_ICON_MOUSEENTER_COLOR
#define BUTTON_ICON_MOUSEENTER_COLOR 0xffffffff
#endif //!BUTTON_ICON_MOUSEENTER_COLOR
#ifndef BUTTON_ICON_MOUSELEFT_COLOR
#define BUTTON_ICON_MOUSELEFT_COLOR 0xff000000
#endif //!BUTTON_ICON_MOUSELEFT_COLOR
#ifndef CLOSE_BUTTON_WIDTH
#define CLOSE_BUTTON_WIDTH 48
#endif //!CLOSE_BUTTON_WIDTH

struct Config {
	unsigned long background = 0xffe3e3e3;
	float borderRadius = 0;
	int borderWidth = 1;
	unsigned long borderColor = 0xff000000;
};

bool RegisterWindowClass(const WCHAR* lpszClassName, WNDPROC lpfnWndProc)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = lpfnWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = lpszClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	return RegisterClass(&wc);
}

unsigned long GdiColorToRGB(unsigned long color)
{
	return RGB(color >> 16, color >> 8, color & 0xff);
}

void RoundedRect(Gdiplus::GraphicsPath* path, float x, float y, float w, float h, const float borderRadius[4][2])
{
	int roundedCorner = 8;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			if (borderRadius[i][j] <= 0) {
				--roundedCorner;
			}
		}
	}

	if (roundedCorner == 0) {
		path->AddRectangle(Gdiplus::RectF{ x, y, w, h });
	}
	else {
		float ARC_TO_BEZIER = 0.55228475f;

		float rx = borderRadius[0][0];
		float ry = borderRadius[0][1];

		if (rx > w / 2) {
			rx = w / 2;
		}
		else if (rx < 0) {
			rx = 0;
		}

		if (ry > h / 2) {
			ry = h / 2;
		}
		else if (ry < 0) {
			ry = 0;
		}

		float c1 = ARC_TO_BEZIER * rx;
		float c2 = ARC_TO_BEZIER * ry;

		Gdiplus::PointF pts[4] = { {}, {}, {}, {x + (rx > borderRadius[3][0] ? rx : borderRadius[3][0]), y} };

		path->AddLine(pts[3], { x + w - rx, y });
		pts[0] = { x + w - rx, y }; pts[1] = { x + w - rx + c1, y }; pts[2] = { x + w, y + c2 }; pts[3] = { x + w, y + ry };
		path->AddBezier(pts[0], pts[1], pts[2], pts[3]);

		rx = borderRadius[1][0];
		ry = borderRadius[1][1];

		if (rx > w / 2) {
			rx = w / 2;
		}
		if (ry > h / 2) {
			ry = h / 2;
		}

		c1 = ARC_TO_BEZIER * rx;
		c2 = ARC_TO_BEZIER * ry;

		path->AddLine(pts[3], { x + w, y + h - ry });
		pts[0] = { x + w, y + h - ry }; pts[1] = { x + w, y + h - ry + c2 }; pts[2] = { x + w + c1 - rx, y + h }; pts[3] = { x + w - rx, y + h };
		path->AddBezier(pts[0], pts[1], pts[2], pts[3]);

		rx = borderRadius[2][0];
		ry = borderRadius[2][1];

		if (rx > w / 2) {
			rx = w / 2;
		}
		if (ry > h / 2) {
			ry = h / 2;
		}

		c1 = ARC_TO_BEZIER * rx;
		c2 = ARC_TO_BEZIER * ry;

		path->AddLine(pts[3], { x + rx, y + h });
		pts[0] = { x + rx, y + h }; pts[1] = { x + rx - c1, y + h }; pts[2] = { x, y + h - c2 }; pts[3] = { x, y + h - ry };
		path->AddBezier(pts[0], pts[1], pts[2], pts[3]);

		rx = borderRadius[3][0];
		ry = borderRadius[3][1];

		if (rx > w / 2) {
			rx = w / 2;
		}
		if (ry > h / 2) {
			ry = h / 2;
		}

		c1 = ARC_TO_BEZIER * rx;
		c2 = ARC_TO_BEZIER * ry;

		path->AddLine(pts[3], { x, y + ry });
		pts[0] = { x, y + ry }; pts[1] = { x, y + ry - c2 }; pts[2] = { x + rx - c1, y }; pts[3] = { x + rx, y };
		path->AddBezier(pts[0], pts[1], pts[2], pts[3]);
	}
}

//Draw rounded rect: https://cairographics.org/cookbook/roundedrectangles/
void RoundedRect(Gdiplus::GraphicsPath* path, float x, float y, float w, float h, float rx)
{
	if (rx < 0) {
		path->AddRectangle(Gdiplus::RectF{ x, y, w, h });
	}
	else {
		if (rx > w / 2) {
			rx = w / 2;
		}

		const float borderRadius[4][2] = { {rx, rx}, {rx, rx}, {rx, rx}, {rx, rx} };
		RoundedRect(path, x, y, w, h, borderRadius);
	}

	path->CloseFigure();
}

void RoundedRect(Gdiplus::GraphicsPath* path, float x, float y, float w, float h, float tl, float tr, float br, float bl)
{
	const float borderRadius[4][2] = { {tr, tr}, {br, br}, {bl, bl}, {tl, tl} };
	RoundedRect(path, x, y, w, h, borderRadius);

	path->CloseFigure();
}

void DrawCloseButton(HWND hwnd, bool isMouseLeave)
{
	HWND parent = GetAncestor(hwnd, GA_PARENT);
	if (parent == nullptr) {
		return;
	}

	Config* config = (Config*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (config == nullptr) {
		return;
	}

	RECT parentRc;
	GetWindowRect(parent, &parentRc);
	RECT rc;
	GetWindowRect(hwnd, &rc);
	OffsetRect(&rc, -parentRc.left, -parentRc.top);
	SIZE size = { rc.right - rc.left, rc.bottom - rc.top };

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	auto bitmap = std::make_unique<Gdiplus::Bitmap>(size.cx, size.cy);
	auto g = std::make_unique<Gdiplus::Graphics>(bitmap.get());
	g->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

	auto path = std::make_unique<Gdiplus::GraphicsPath>();
	RoundedRect(path.get(), 0, 0, (float)size.cx, (float)size.cy, 0, config->borderRadius - config->borderWidth * 2, 0, 0);
	std::unique_ptr<Gdiplus::Brush> br = std::make_unique<Gdiplus::SolidBrush>(isMouseLeave ? 0x01000000 : 0xffE81123);
	g->FillPath(br.get(), path.get());
	br.reset();
	path.reset();

	float iconSize = 10;
	auto pen = std::make_unique<Gdiplus::Pen>(isMouseLeave ? BUTTON_ICON_MOUSELEFT_COLOR : BUTTON_ICON_MOUSEENTER_COLOR, 1.0f);
	g->DrawLine(pen.get(), Gdiplus::PointF{ size.cx / 2 - iconSize / 2, size.cy / 2 - iconSize / 2 }, Gdiplus::PointF{ size.cx / 2 + iconSize / 2, size.cy / 2 + iconSize / 2 });
	g->DrawLine(pen.get(), Gdiplus::PointF{ size.cx / 2 + iconSize / 2, size.cy / 2 - iconSize / 2 }, Gdiplus::PointF{ size.cx / 2 - iconSize / 2, size.cy / 2 + iconSize / 2 });
	pen.reset();

	HDC screenDc = GetWindowDC(nullptr);
	HDC memDc = CreateCompatibleDC(screenDc);
	HBITMAP hBitmap = nullptr;
	bitmap->GetHBITMAP(Gdiplus::Color(0), &hBitmap);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDc, hBitmap);
	POINT pointSource = { 0, 0 };
	POINT topPos = { rc.left, rc.top };
	BLENDFUNCTION blend{};
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	UpdateLayeredWindow(hwnd, screenDc, &topPos, &size, memDc, &pointSource, 0, &blend, 0x00000002);
	ReleaseDC(nullptr, screenDc);
	SelectObject(memDc, oldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(memDc);

	g->Flush();
	g.reset();
	bitmap.reset();

	Gdiplus::GdiplusShutdown(token);
}

static LRESULT CALLBACK CloseButtonProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static bool isMouseLeave = true;
	switch (msg)
	{
	case WM_MOUSEMOVE:
	{
		if (isMouseLeave) {
			TRACKMOUSEEVENT tme{};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 1;
			tme.hwndTrack = hwnd;
			TrackMouseEvent(&tme);
			isMouseLeave = false;
			DrawCloseButton(hwnd, isMouseLeave);
		}
	} break;
	case WM_MOUSELEAVE:
	{
		if (!isMouseLeave) {
			isMouseLeave = true;
			DrawCloseButton(hwnd, isMouseLeave);
		}
	} break;
	case WM_LBUTTONDOWN:
	{
		HWND parent = GetAncestor(hwnd, GA_PARENT);
		if (parent != nullptr) {
			SendMessage(parent, WM_CLOSE, 0, 0);
		}
	} break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return 0;
}

void CreateCloseButton(HWND parentWindow, const Config& config)
{
	RECT rc;
	GetClientRect(parentWindow, &rc);
	OffsetRect(&rc, -rc.left, -rc.top);
	RegisterWindowClass(L"CloseButton", CloseButtonProc);
	HWND closeButton = CreateWindowEx(WS_EX_LAYERED, L"CloseButton", nullptr, WS_POPUP | WS_CHILD | WS_VISIBLE, rc.right - CLOSE_BUTTON_WIDTH - config.borderWidth, config.borderWidth, CLOSE_BUTTON_WIDTH, TITLEBAR_HEIGHT - config.borderWidth, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr(closeButton, GWLP_USERDATA, (LONG_PTR)&config);
	SetParent(closeButton, parentWindow);
	DrawCloseButton(closeButton, true);
}

void DrawBaseWindow(HWND hwnd, bool isFocus)
{
	HWND parent = GetAncestor(hwnd, GA_PARENT);
	if (parent == nullptr) {
		return;
	}

	Config* config = (Config*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (config == nullptr) {
		return;
	}

	RECT parentRc;
	GetWindowRect(parent, &parentRc);
	RECT rc;
	GetWindowRect(hwnd, &rc);
	OffsetRect(&rc, -parentRc.left, -parentRc.top);
	SIZE size = { rc.right - rc.left, rc.bottom - rc.top };

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	auto bitmap = std::make_unique<Gdiplus::Bitmap>(size.cx, size.cy);
	auto g = std::make_unique<Gdiplus::Graphics>(bitmap.get());
	g->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

	auto path = std::make_unique<Gdiplus::GraphicsPath>();
	RoundedRect(path.get(), 0, 0, (float)size.cx, (float)size.cy, config->borderRadius);

	auto br = std::make_unique<Gdiplus::SolidBrush>(config->background);
	g->SetClip(Gdiplus::RectF{ 0, size.cy - config->borderRadius * 2, (float)size.cx, config->borderRadius * 2 });
	g->FillPath(br.get(), path.get());
	g->ResetClip();
	br.reset();

	br = std::make_unique<Gdiplus::SolidBrush>(isFocus ? TITLEBAR_ACTIVE_COLOR : TITLEBAR_INACTIVE_COLOR);
	g->SetClip(Gdiplus::Rect{ 0, 0, size.cx, TITLEBAR_HEIGHT });
	g->FillPath(br.get(), path.get());
	g->ResetClip();
	br.reset();

	path.reset();

	if (config->borderWidth > 0) {
		path = std::make_unique<Gdiplus::GraphicsPath>();
		RoundedRect(path.get(), config->borderWidth / 2.0f, config->borderWidth / 2.0f, size.cx - config->borderWidth, size.cy - config->borderWidth, config->borderRadius - config->borderWidth);
		auto p = std::make_unique<Gdiplus::Pen>(config->borderColor, (float)config->borderWidth);
		g->DrawPath(p.get(), path.get());
		p.reset();
		path.reset();
	}

	HDC screenDc = GetWindowDC(nullptr);
	HDC memDc = CreateCompatibleDC(screenDc);
	HBITMAP hBitmap = nullptr;
	bitmap->GetHBITMAP(Gdiplus::Color(0), &hBitmap);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDc, hBitmap);
	POINT pointSource = { 0, 0 };
	POINT topPos = { rc.left, rc.top };
	BLENDFUNCTION blend{};
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	UpdateLayeredWindow(hwnd, screenDc, &topPos, &size, memDc, &pointSource, 0, &blend, 0x00000002);
	ReleaseDC(nullptr, screenDc);
	SelectObject(memDc, oldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(memDc);

	g->Flush();
	g.reset();
	bitmap.reset();

	Gdiplus::GdiplusShutdown(token);
}

static LRESULT BaseWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static POINT startDragPoint;
	static bool isMouseDownTitlebar = false;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		POINT e;
		if (GetCursorPos(&e)) {
			if (ScreenToClient(hwnd, &e)) {
				if (e.y <= TITLEBAR_HEIGHT) {
					isMouseDownTitlebar = true;
					GetCursorPos(&startDragPoint);
					SetCapture(hwnd);
				}
			}
		}
	} break;
	case WM_MOUSEMOVE:
	{
		if (isMouseDownTitlebar) {
			HWND emptyHwnd = GetAncestor(hwnd, GA_PARENT);
			if (emptyHwnd != nullptr) {
				POINT currentPos;
				GetCursorPos(&currentPos);
				int deltaX = currentPos.x - startDragPoint.x;
				int deltaY = currentPos.y - startDragPoint.y;
				RECT parentRect;
				GetWindowRect(emptyHwnd, &parentRect);
				int newX = parentRect.left + deltaX;
				int newY = parentRect.top + deltaY;
				SetWindowPos(emptyHwnd, nullptr, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				startDragPoint = currentPos;
			}
		}
	} break;
	case WM_LBUTTONUP:
	{
		isMouseDownTitlebar = false;
		ReleaseCapture();
	} break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

void CreateBaseWindow(HWND parentWindow, const Config& config)
{
	RECT rc;
	GetWindowRect(parentWindow, &rc);
	OffsetRect(&rc, -rc.left, -rc.top);
	RegisterWindowClass(L"BaseWindow", BaseWindowProc);
	HWND baseWindow = CreateWindowEx(WS_EX_LAYERED, L"BaseWindow", nullptr, WS_POPUP | WS_CHILD | WS_VISIBLE, 0, 0, rc.right, rc.bottom, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr(baseWindow, GWLP_USERDATA, (LONG_PTR)&config);
	SetParent(baseWindow, parentWindow);
	DrawBaseWindow(baseWindow, true);
	// SetWindowPos(baseWindow, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

static LRESULT EmptyWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

HWND InitWindow(int width, int height, const WCHAR* title, const Config& config) {
	SIZE emptyWindowSz{ width + 2 * config.borderWidth, height + TITLEBAR_HEIGHT + config.borderWidth + (int)config.borderRadius };

	RegisterWindowClass(L"EmptyWindow", EmptyWindowProc);
	HWND emptyWindow = CreateWindow(L"EmptyWindow", nullptr, WS_POPUP, 0, 0, emptyWindowSz.cx, emptyWindowSz.cy, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	ShowWindow(emptyWindow, SW_SHOW);

	CreateBaseWindow(emptyWindow, config);
	CreateCloseButton(emptyWindow, config);

	RegisterWindowClass(L"MainWindow", DefWindowProc);
	HWND mainWindow = CreateWindow(L"MainWindow", nullptr, WS_CHILD | WS_VISIBLE, config.borderWidth, TITLEBAR_HEIGHT, width, height, emptyWindow, nullptr, GetModuleHandle(nullptr), nullptr);
	SetClassLongPtr(mainWindow, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(GdiColorToRGB(config.background)));

	return mainWindow;
}

#endif // !GDOW_H
