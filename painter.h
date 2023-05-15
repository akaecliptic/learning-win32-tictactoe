#ifndef XO_ONCE
#define XO_ONCE

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "logic.h"

template<class T> 
void SafeRelease(T**);

namespace Painter
{

struct CRISS_CROSS
{
    D2D1_POINT_2F center;
    D2D1_POINT_2F start1;
    D2D1_POINT_2F end1;
    D2D1_POINT_2F start2;
    D2D1_POINT_2F end2;
};

struct GRID_RECT
{
    D2D1_POINT_2F center;
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;

    INT player = 0;
};

struct LINE
{
    D2D_POINT_2F start;
    D2D_POINT_2F end;
};

CRISS_CROSS CrissCross(const D2D1_POINT_2F&, FLOAT);
GRID_RECT Rect(FLOAT, FLOAT, FLOAT, FLOAT);
void Rect(FLOAT, FLOAT, FLOAT, FLOAT, GRID_RECT&);

class XO
{
    public:
        static float shapeSize;
        
    private:
        HWND& _parent;
        ID2D1Factory* _factory;
        ID2D1HwndRenderTarget* _target;
        ID2D1SolidColorBrush* _brush;

        int8_t _state;

        LINE _border[4];
        GRID_RECT _grid[9];

    public:
        XO(HWND&);

        void Render();

        void TakeTurn();
        void CheckBoundaries(INT, INT);
        
        void EndGame();
        void Restart();

        void    CalculateLayout();
        HRESULT CreateGraphicsResources();
        void    DiscardGraphicsResources();
        void    OnPaint();
        void    OnResize();

        HRESULT CreateFactory();
        void DiscardFactory();
};

}

#endif
