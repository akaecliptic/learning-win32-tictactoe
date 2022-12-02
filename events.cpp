#include "events.h"

namespace Events
{

Dispatcher::Dispatcher() : _msg({}) {}

SeekDispatcher::SeekDispatcher() : Dispatcher() {}

BOOL SeekDispatcher::Next()
{
    return GetMessage(&_msg, NULL, 0, 0);
}

BOOL SeekDispatcher::Dispatch()
{
    TranslateMessage(&_msg);
    DispatchMessage(&_msg);

    return 0;
}

}
