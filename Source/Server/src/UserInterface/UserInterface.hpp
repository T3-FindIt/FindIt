#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <MessageQueue.hpp>
#include <IMessage.hpp>
#include <ServerRequestProduct.hpp>
#include <ItemType.hpp>

namespace FindIt
{

using Get_objects_t = std::function<std::vector<FindIt::ItemType>()>;
using Add_objects_Database_t = std::function<void(FindIt::ItemType)>;

class UserInterface
{
private:
    void PrintStartUp(int *choice);
    Get_objects_t ReturnUniqueObjectsTypes;
    Add_objects_Database_t AddObject;
    FindIt::MessageQueue &QueueIn;
    FindIt::MessageQueue &QueueOut;
    bool running;

public:
    UserInterface(Get_objects_t functionPointer, Add_objects_Database_t functionpointer3 ,FindIt::MessageQueue &queueIn, FindIt::MessageQueue &queueOut);
    ~UserInterface() = default;

    void Run();
    void Stop();
};

};

#endif // USERINTERFACE_HPP
