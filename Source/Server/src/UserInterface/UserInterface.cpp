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
        int Choice = 0;
        std::cout << "Welcome to the FindIt Server UI, please select your choice:\n"
                  << "1. Show all known object types.\n"
                  << "2. Request/search up objects.\n"
                  << "3. Add object to the Database. \n"
                  << "4. Quit application." << std::endl;
        std::cin >> Choice;
        // TODO: Goes wrong here?
        *choice = Choice;
    }

    void UserInterface::Run()
    {
        int choice = 0;
        int ID = 1; // dirty hardcoded fix
        running = true;

        while (running)
        {
            PrintStartUp(&choice);
            switch (choice)
            {
            case 1:
            {
                std::cout << "All types:" << std::endl;
                for (auto type : ReturnUniqueObjectsTypes())
                {
                    std::cout << type.GetName() << std::endl;
                }
                break;
            }
            case 2:
            {
                std::cout << "Type in the object you would like to request:" << std::endl;
                std::string requestedObject;
                std::cin >> requestedObject;
                // RequestObject(requestedObject);
                std::shared_ptr<FindIt::IMessage> msg = std::make_shared<FindIt::ServerRequestProduct>(requestedObject, true);
                QueueOut.push(msg);
                std::cout << "Unfortunatly we cannot tell you if the object was found or not." << std::endl;
                system("cls");
                break;
            }
            case 3:
            {
                std::cout << "Type in the object you would like to add:\n"
                            << "The first name without spaces will be used!" << std::endl;
                std::string ObjectToBeAdded;
                std::cin >> ObjectToBeAdded;
                FindIt::ItemType obj = FindIt::ItemType(ObjectToBeAdded, ID);
                AddObject(obj);
                system("cls");
                break;
            }
            case 4:
            {
                std::cout << "quitting" << std::endl;
                UserInterface::Stop();
                system("cls");
                break;
            }

            default:
            {
                std::cout << "something went wrong" << std::endl;
                system("cls");
                break;
            }
            }
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }

    void UserInterface::Stop()
    {
        running = false;
    }
};
