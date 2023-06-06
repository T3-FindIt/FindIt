#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <../Message/MessageQueue/MessageQueue.hpp>

namespace FindIt
{

using Get_objects_t = std::function<std::vector<std::string>()>;
using Request_object_t = std::function<void(std::string)>;

class UserInterface
{
private:
    void PrintStartUp(int *choice);
    Get_objects_t ReturnUniqueObjectsTypes;
    Request_object_t RequestObject;
    FindIt::MessageQueue &Queue;
    bool running;
    // std::thread UIthread(Run);
    //moet de thread niet in main.cpp aangemaakt worden?
    //zo niet waar dan wel?

public:
    UserInterface(Get_objects_t functionPointer, Request_object_t functionpointer2, FindIt::MessageQueue &queue);
    ~UserInterface();

    void Run();
    void Stop();
};

};

#endif // USERINTERFACE_HPP
