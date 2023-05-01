#ifndef ICLUSTERCONNECTION_HPP
#define ICLUSTERCONNECTION_HPP

#include <IMessage.hpp>

#include <functional>

namespace FindIt
{

using Event_function_t = std::function<void(std::string)>;

class IClusterConnection
{
protected:
    Event_function_t onMessageHandler;
    bool isRunning;
public:
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual void Broadcast(std::string& message) = 0;
    virtual void SetOnMessageHandler(Event_function_t handler) = 0;
};

};

#endif // ICLUSTERCONNECTION_HPP
