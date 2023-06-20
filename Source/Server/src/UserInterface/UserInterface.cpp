#include "UserInterface.hpp"

namespace FindIt
{
    UserInterface::UserInterface(Get_objects_t functionPointer
                                , Add_objects_Database_t functionpointer3
                                , FindIt::MessageQueue &queueIn
                                , FindIt::MessageQueue &queueOut)
        : ReturnUniqueObjectsTypes(functionPointer)
        , AddObject(functionpointer3)
        , QueueIn(queueIn)
        , QueueOut(queueOut)
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
        *choice = Choice;
    }

    void UserInterface::Run()
    {
        int choice = 0;
        running = true;

        while (running)
        {
            PrintStartUp(&choice);
            switch (choice)
            {
            case 1:
            {
                system("cls");
                std::cout << "All types:" << std::endl;
                for (const auto& type : ReturnUniqueObjectsTypes())
                {
                    std::cout << type.GetName() << std::endl;
                }
                break;
            }
            case 2:
            {
                system("cls");
                std::cout << "All types:" << std::endl;
                for (const auto& type : ReturnUniqueObjectsTypes())
                {
                    std::cout << type.GetName() << std::endl;
                }
                std::cout << std::endl;
                std::cout << "Type in the object you would like to request:" << std::endl;
                std::string requestedObject;
                std::cin >> requestedObject;
                // check if the database has the object
                bool found = false;
                for (const auto& type : ReturnUniqueObjectsTypes())
                {
                    if (type.GetName() == requestedObject)
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    std::cout << "The object you requested is not in the database." << std::endl;
                    std::cout << "If you still want to request it, it will be added to the database." << std::endl;
                    std::cout << "Do you want to continue? y/n" << std::endl;
                    char answer;
                    std::cin >> answer;
                    answer = tolower(answer);
                    if (answer == 'y')
                    {
                        FindIt::ItemType obj = FindIt::ItemType(requestedObject);
                        AddObject(obj);
                    }
                    else
                    {
                        break;
                    }
                }

                std::shared_ptr<FindIt::IMessage> msg = std::make_shared<FindIt::ServerRequestProduct>(requestedObject, true);
                QueueOut.push(msg);
                std::cout << "Unfortunatly we cannot tell you if the object was found or not.\n\n\n\n" << std::endl;
                break;
            }
            case 3:
            {
                system("cls");
                std::cout << "Type in the object you would like to add:\n"
                        << "The first name without spaces will be used!" << std::endl;
                std::string ObjectToBeAdded;
                std::cin >> ObjectToBeAdded;
                FindIt::ItemType obj = FindIt::ItemType(ObjectToBeAdded);
                AddObject(obj);
                system("cls");
                break;
            }
            case 4:
            {
                system("cls");
                std::cout << "quitting..." << std::endl;
                UserInterface::Stop();
                break;
            }
            default:
            {
                system("cls");
                std::cout << "something went wrong" << std::endl;
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
