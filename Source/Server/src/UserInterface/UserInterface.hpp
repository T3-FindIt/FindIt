#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <../Message/MessageQueue/MessageQueue.hpp>
#include <../Message/IMessage.hpp>
#include <ServerRequestProduct.hpp>
#include <ItemType.hpp>

namespace FindIt
{

using Get_objects_t = std::function<std::vector<FindIt::ItemType>()>;
using Request_object_t = std::function<void(std::string)>;
using Add_objects_Database_t = std::function<void(FindIt::ItemType)>;

class UserInterface
{
private:
    void PrintStartUp(int *choice);
    Get_objects_t ReturnUniqueObjectsTypes;
    Request_object_t RequestObject;
    Add_objects_Database_t AddObject;
    FindIt::MessageQueue &QueueIn;
    FindIt::MessageQueue &QueueOut;
    bool running;

public:
    UserInterface(Get_objects_t functionPointer, Request_object_t functionpointer2, Add_objects_Database_t functionpointer3 ,FindIt::MessageQueue &queueIn, FindIt::MessageQueue &queueOut);
    ~UserInterface() = default;

    void Run();
    void Stop();
};

};

#endif // USERINTERFACE_HPP
