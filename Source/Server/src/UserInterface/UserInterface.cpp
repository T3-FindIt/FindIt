#include "UserInterface.hpp"
// update design (extra functionpointer) -> no time

namespace FindIt
{
    UserInterface::UserInterface(Get_objects_t functionPointer, Request_object_t functionpointer2, Add_objects_Database_t functionpointer3, FindIt::MessageQueue &queueIn, FindIt::MessageQueue &queueOut)
        : ReturnUniqueObjectsTypes(functionPointer), RequestObject(functionpointer2), AddObject(functionpointer3), QueueIn(queueIn), QueueOut(queueOut)
    {
    }

    void UserInterface::PrintStartUp(int *choice)
    {
        std::cout << "Welcome to the FindIt Server UI, please select your choice:\n"
                  << "1. Show all known object types.\n"
                  << "2. Request/search up objects.\n"
                  << "3. Add object to the Database. \n"
                  << "4. Quit application." << std::endl;
        std::cin >> *choice;
    }

    void UserInterface::Run()
    {
        int choice = 0;
        std::string requestedObject;
        std::string ObjectToBeAdded;
        std::shared_ptr<FindIt::IMessage> msg;
        std::shared_ptr<FindIt::ItemType> obj;

        int ID = 1; // dirty hardcoded fix
        running = true;

        while (running)
        {
            PrintStartUp(&choice);
            switch (choice)
            {
            case 1:
                std::cout << "All types:" << std::endl;
                for (auto type : ReturnUniqueObjectsTypes())
                {
                    std::cout << type << std::endl;
                }
                break;

            case 2:
                std::cout << "Type in the object you would like to request:" << std::endl;
                std::cin >> requestedObject;
                RequestObject(requestedObject);
                msg = std::make_shared<FindIt::ServerRequestProduct>(requestedObject, true);
                QueueOut.push(msg);
                break;

            case 3:
                std::cout << "Type in the object you would like to add:" << std::endl;
                std::cin >> ObjectToBeAdded;
                obj = std::make_shared<FindIt::ItemType>(ObjectToBeAdded, ID);
                AddObject((*obj));
                obj.reset();
                break;

            case 4:
                std::cout << "quitting";
                msg = nullptr;
                obj = nullptr;
                UserInterface::Stop();
                break;

            default:
                std::cout << "something went wrong";
                break;
            }
        }
    }

    void UserInterface::Stop()
    {
        running = false;
    }
};
