// #include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <PlainFileDatabase.hpp>
#include <filesystem>
#include <fstream>

#include "./UserInterface/UserInterface.hpp"

FindIt::PlainFileDatabase database = FindIt::PlainFileDatabase("DatabaseTestFiles/database.db");
FindIt::MessageQueue msgq1 = FindIt::MessageQueue();
FindIt::MessageQueue msgq2 = FindIt::MessageQueue();

int main()
{
    FindIt::UserInterface *UI = new FindIt::UserInterface(std::bind_front(&FindIt::PlainFileDatabase::GetAllObjects, &database)
                                                            , std::bind_front(&FindIt::PlainFileDatabase::Add, &database)
                                                            , msgq1
                                                            , msgq2);

    std::jthread t1(&FindIt::UserInterface::Run, UI);
    t1.join();

    delete UI;
}
