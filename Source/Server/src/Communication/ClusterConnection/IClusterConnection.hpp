#ifndef ICLUSTERCONNECTION_HPP
#define ICLUSTERCONNECTION_HPP

#include "../../Message/IMessage.hpp"

namespace FindIt
{
    using Event_function_t = void();
    
    class IClusterConnection
    {
    private:
        Event_function_t onMessageHandler;
    public:
        virtual [[noreturn]] void Run() = 0;
        virtual void Stop() = 0;
        virtual void Broadcast(iMessage message) = 0;
        virtual void SetOnMessagehandler(Event_function_t handler) = 0;
    };
};

#endif //ICLUSTERCONNECTION_HPP
