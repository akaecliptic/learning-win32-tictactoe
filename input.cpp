#include "input.h"

namespace Input
{

Manager& Manager::Instance()
{
    static Manager instance;
    return instance;
}

Manager::Manager() {}

MouseData Manager::ReadMouse(WPARAM wparam, LPARAM lparam)
{
    INT x = GET_X_LPARAM(lparam); 
    INT y = GET_Y_LPARAM(lparam);
    UINT button = GET_XBUTTON_WPARAM(wparam);
    return { x, y, button };
}

KeyboardData Manager::ReadKeyBoard(WPARAM wparam)
{
    return { (CHAR) wparam };
}

}
