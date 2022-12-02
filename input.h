#ifndef INPUT_ONCE
#define INPUT_ONCE
#endif

#include <windows.h>
#include <windowsx.h>

namespace Input
{

struct MouseData
{
    INT x;
    INT y;
    UINT button;
};

struct KeyboardData
{
    CHAR key;
};

class Manager 
{

    public: 
        static Manager& Instance();
    private:
        Manager();

    public:
        Manager(Manager const&) = delete;
        void operator=(Manager const&) = delete;

        MouseData ReadMouse(WPARAM, LPARAM);
        KeyboardData ReadKeyBoard(WPARAM);

};

}
