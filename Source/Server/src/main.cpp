// #include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

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

int main()
{
    FindIt::MessageQueue queue;
    FindIt::UserInterface *UI = new FindIt::UserInterface(testfunc, lol, queue);

    std::jthread t1(&FindIt::UserInterface::Run, UI);
    t1.join();

    delete UI;

    return 0;
}
