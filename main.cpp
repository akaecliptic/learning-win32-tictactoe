#include "main.h"

static LRESULT Handler(Frama::Fuinneog*, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR args, int show)
{
    Frama::Fuinneog window;
    window.SetMessageHandler(Handler);

    if (!window.Create(L"Tic-Tac-Toe")) return 0;

    // Run the message loop.

    Events::SeekDispatcher dispatcher;

    while(dispatcher.Next())
    {
        dispatcher.Dispatch();
    }

    return 0;
}


LRESULT Handler(Frama::Fuinneog* window, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            if (FAILED(window->_xos.CreateFactory())) return -1;  // Fail CreateWindowEx.
            return 0;

        case WM_DESTROY:
            window->_xos.DiscardGraphicsResources();
            window->_xos.DiscardFactory();
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            window->_xos.OnPaint();
            return 0;

        case WM_SIZE:
            window->_xos.OnResize();
            return 0;

        case WM_LBUTTONDOWN:
            Input::MouseData data = Input::Manager::Instance().ReadMouse(wParam, lParam);
            window->_xos.CheckBoundaries(data.x, data.y);
            window->_xos.TakeTurn();
            return 0;
    }

    return DefWindowProc(window->_hwnd, uMsg, wParam, lParam);
}


