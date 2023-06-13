// #include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <PlainFileDatabase.hpp>
#include <filesystem>
#include <fstream>

#include "./UserInterface/UserInterface.hpp"

std::vector<std::string> testfunc()
{
    std::vector<std::string> test;
    test.push_back("string 1 ");
    test.push_back("string 2 ");
    test.push_back("string 3 ");
    test.push_back("string 4 ");
    test.push_back("string 5 ");
    test.push_back("string 6 ");
    test.push_back("string 7 ");

    return test;
    //test functionality
}

void lol(std::string xd)
{
    std::cout << "lol function called" << std::endl;
    //test functionality
}

FindIt::PlainFileDatabase database = FindIt::PlainFileDatabase("DatabaseTestFiles/database.db");
FindIt::MessageQueue msgq1 = FindIt::MessageQueue();
FindIt::MessageQueue msgq2 = FindIt::MessageQueue();

void add(FindIt::ItemType obj)
{
    database.Add(obj);
}

int main()
{ 
    FindIt::UserInterface *UI = new FindIt::UserInterface(testfunc, lol, add, msgq1, msgq2);

    std::jthread t1(&FindIt::UserInterface::Run, UI);
    t1.join();

    delete UI;
}
