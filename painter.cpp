#include "painter.h"

template <class T> 
void SafeRelease(T** com)
{
    if (*com)
    {
        (*com)->Release();
        *com = NULL;
    }
}

namespace Painter
{

float XO::shapeSize = 0;

// Namespace Functions

CRISS_CROSS CrissCross(const D2D1_POINT_2F& center, FLOAT length)
{
    CRISS_CROSS out;
    out.center = center;

    float arm = 0.71f * length; //(sin(45)/cos(45)) * Hyp = x and y coords 

    out.start1 = D2D1::Point2F(center.x - arm, center.y - arm);
    out.end1 = D2D1::Point2F(center.x + arm, center.y + arm);

    out.start2 = D2D1::Point2F(center.x + arm, center.y - arm);
    out.end2 = D2D1::Point2F(center.x - arm, center.y + arm);

    return out;
}

GRID_RECT Rect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    D2D1_POINT_2F center = D2D1::Point2F(left + ((right - left) / 2), top + ((bottom - top) / 2));
    return { center, left, top, right, bottom };
}

void Rect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, GRID_RECT& ref)
{
    ref.center = D2D1::Point2F(left + ((right - left) / 2), top + ((bottom - top) / 2));

    ref.left = left;
    ref.top = top;
    ref.right = right;
    ref.bottom = bottom;
}

// Class Methods

XO::XO(HWND& parent)
    : _parent(parent), _factory(NULL), _target(NULL), _brush(NULL), _state(0)
{ }

void XO::Render()
{
    HRESULT result = CreateGraphicsResources();
    if(SUCCEEDED(result))
    {
        PAINTSTRUCT paint;
        BeginPaint(_parent, &paint);

        _target->BeginDraw();
        _target->Clear( D2D1::ColorF(D2D1::ColorF::AliceBlue) );

        for (size_t i = 0; i < 4; i++)
            _target->DrawLine(_border[i].start, _border[i].end, _brush, 5.0f);
        
        for (size_t i = 0; i < 9; i++)
        {
            if ( _grid[i].player == -1 )
            {
                D2D1_ELLIPSE oh = D2D1::Ellipse(_grid[i].center, XO::shapeSize, XO::shapeSize);
                _target->DrawEllipse(oh, _brush, 5.0f);
            } 
            else if ( _grid[i].player == 1 )
            {
                CRISS_CROSS ex = CrissCross(_grid[i].center, XO::shapeSize);
                _target->DrawLine(ex.start1, ex.end1, _brush, 5.0f);
                _target->DrawLine(ex.start2, ex.end2, _brush, 5.0f);
            }
        }

        result = _target->EndDraw();
        if(FAILED(result) || result == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }

        EndPaint(_parent, &paint);
    }
}

void XO::TakeTurn()
{
    Render();
    if (_state != 0) EndGame();
}

void XO::CheckBoundaries(INT x, INT y)
{
    for (size_t i = 0; i < 9; i++)
    {
        if ( x >= _grid[i].left && x <= _grid[i].right && y >= _grid[i].top && y <= _grid[i].bottom )
        {
            if ( _grid[i].player != 0) return;

            _grid[i].player = Logic::turn;
            Logic::grid[i/3][i % 3] = Logic::turn;
            _state = Logic::TakeTurn();
        }
    }
}

void XO::EndGame()
{
    LPCWSTR message;

    if (_state == INVALID_MOVES)
    {
        message = L"A Draw. Do you want to play again?";
    } else {
        message = (_state == 1) ? 
            L"Xs Win. Do you want to play again?" :
            L"Os Win. Do you want to play again?";
    }

    int response = MessageBox(_parent, message, L"Game Over", MB_YESNO);

    if (response == IDYES) Restart();
    else DestroyWindow(_parent);
}

void XO::Restart()
{
    Logic::Restart();
    
    _state = 0;
    XO::shapeSize = 0;

    CalculateLayout();
    Render();
}

void XO::CalculateLayout()
{
    if(_target != NULL)
    {
        D2D1_SIZE_F size = _target->GetSize();
        const float unitX = size.width / 3;
        const float unitY = size.height / 3;

        for (size_t i = 0; i < 2; i++)
        {
            _border[i] = { { unitX + (unitX * i), 0 }, { unitX + (unitX * i), size.height } };
            _border[i + 2] = { { 0, unitY + (unitY * i) }, { size.width, unitY + (unitY * i) } };
        }

        if(XO::shapeSize == 0) 
        {
            for (size_t i = 0; i < 3; i++)    
                for (size_t j = 0; j < 3; j++)
                    _grid[(i * 3) + j] = Rect(
                        0.0f + (unitX * j), 0.0f + (unitY * i), 
                        unitX + (unitX * j), unitY + (unitY * i)
                    ); 
        } else {
            for (size_t i = 0; i < 3; i++)    
                for (size_t j = 0; j < 3; j++)
                    Rect(
                        0.0f + (unitX * j), 0.0f + (unitY * i), 
                        unitX + (unitX * j), unitY + (unitY * i),
                        _grid[(i * 3) + j]
                    ); 
        }
        
        XO::shapeSize = (( size.width < size.height ) ? size.width : size.height) / 7;
    }
}

HRESULT XO::CreateGraphicsResources()
{
    HRESULT result = S_OK;

    if(_target == NULL)
    {
        RECT rect;
        GetClientRect(_parent, &rect);

        D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

        result = _factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(_parent, size),
            &(this->_target)  
        );

        if(SUCCEEDED(result))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Black);
            result = _target->CreateSolidColorBrush(color, &_brush);

            if(SUCCEEDED(result)) { CalculateLayout(); }
        }
    }

    return result;
}

void XO::DiscardGraphicsResources()
{
    SafeRelease(&_target);
    SafeRelease(&_brush);
}

void XO::OnPaint()
{
    Render();
}

void XO::OnResize()
{
    if(_target != NULL)
    {
        RECT rect;
        GetClientRect(_parent, &rect);

        D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

        _target->Resize(size);
        CalculateLayout();
        InvalidateRect(_parent, NULL, FALSE);
    }
}

HRESULT XO::CreateFactory() 
{
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory);
}

void XO::DiscardFactory() 
{
    SafeRelease(&_factory);
}

}
