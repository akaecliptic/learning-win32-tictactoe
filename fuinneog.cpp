#include "fuinneog.h"

namespace Frama
{

LRESULT CALLBACK Fuinneog::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Fuinneog* reflection = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        reflection = (Fuinneog*) pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) reflection);

        reflection->_hwnd = hwnd;
    } else {
        reflection = (Fuinneog*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (reflection && reflection->_handler)
    {
        return reflection->_handler(reflection, uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

Fuinneog::Fuinneog() : _hwnd(NULL), _windowClass((PCWSTR)L"Fuinneog App"), _handler(nullptr), _xos(_hwnd) { }

HWND Fuinneog::GetHWND() const { return _hwnd; }
PCWSTR Fuinneog::GetWindowClassName() const { return _windowClass; }

BOOL Fuinneog::SetMessageHandler(MessageHandler handler) { 
    _handler = handler; 
    return !(_handler);    
}

BOOL Fuinneog::Create(
    PCWSTR title,
    DWORD style,
    DWORD exStyle,
    int x,
    int y,
    int width,
    int height,
    HWND parent,
    HMENU menu,
    int show
)
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = Fuinneog::WindowProcedure;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = _windowClass;

    RegisterClass(&wc);

    _hwnd = CreateWindowEx(
        exStyle, _windowClass, title, style, x, y,
        width, height, parent, menu, GetModuleHandle(NULL), this
    );

    BOOL status = (_hwnd ? TRUE : FALSE) && (_handler);

    if (!status) return FALSE; //Consider throwing an Exception. This is to prevent uninstantiated message handler

    ShowWindow(_hwnd, show);
    return TRUE;
}

}
