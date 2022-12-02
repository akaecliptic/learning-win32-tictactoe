#ifndef EVENTS_ONCE
#define EVENTS_ONCE
#endif

#include <windows.h>

namespace Events
{

class Dispatcher
{
    protected:
        MSG _msg;

    protected:
        Dispatcher();

    public:
        virtual BOOL Next() = 0;
        virtual BOOL Dispatch() = 0;
};

class SeekDispatcher : public Dispatcher 
{ 
    public: 
        SeekDispatcher();
        
        BOOL Next() override;
        BOOL Dispatch() override;
};

}
