#ifndef ICLUSTERCONNECTION_HPP
#define ICLUSTERCONNECTION_HPP

#include "../../Message/IMessage.hpp"

#include <functional>

namespace FindIt
{

using Event_function_t = std::function<void()>;

class IClusterConnection
{
private:
    Event_function_t onMessageHandler;
public:
    virtual ~IClusterConnection() = default;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual void Broadcast(IMessage& message) = 0;
    virtual void SetOnMessageHandler(Event_function_t handler) = 0;
};

};

#endif // ICLUSTERCONNECTION_HPP
