#ifndef FRAMA_ONCE
#define FRAMA_ONCE

#include <windows.h>

#include "input.h"
#include "painter.h"

#endif

namespace Frama
{

class Fuinneog
{
    
    public:
        typedef LRESULT (*MessageHandler)(Fuinneog* parent, UINT uMsg, WPARAM wParam, LPARAM lParam);
        
    public:
        HWND _hwnd;
        PCWSTR _windowClass;
        MessageHandler _handler;
        Painter::XO _xos;

    public:
        static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        Fuinneog();

        HWND GetHWND() const;
        PCWSTR GetWindowClassName() const;

        BOOL SetMessageHandler(MessageHandler);
        BOOL Create(
            PCWSTR title = L"Look At That View",
            DWORD style = WS_OVERLAPPEDWINDOW,
            DWORD exStyle = 0,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT,
            HWND parent = NULL,
            HMENU menu = NULL,
            int show = SW_SHOWDEFAULT
        );

};

}
