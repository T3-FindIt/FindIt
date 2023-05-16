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
}

void lol(std::string xd)
{
    std::cout << "lol function called\n";
}

int main()
{
    // std::function<>
    FindIt::UserInterface *UI = new FindIt::UserInterface(testfunc, lol);

    UI->Run();

    // // Set cluster coms to a TCP connection on port 54000
    // FindIt::Server* clusterConnection = new FindIt::Server(54000);

    // // Set protocol parser to JSON
    // FindIt::JSONProtocolParser* protocolParser = new FindIt::JSONProtocolParser();

    // // Set up communication and start a new thread for it
    // FindIt::Communication* communication = new FindIt::Communication(*clusterConnection, *protocolParser);
    // std::jthread communicationThread(&FindIt::Communication::Run, communication);

    //std::cin.get(); //check this out

    // communication->Stop();
    // communicationThread.join();

    delete UI;

    return 0;
}
