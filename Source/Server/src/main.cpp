#include <iostream>
#include <thread>
#include <vector>

#include <Communication.hpp>
#include <TCPConnection.hpp>
#include <JSONProtocolParser.hpp>
#include <Object.hpp>
#include <PlainFileDatabase.hpp>
#include <IMessage.hpp>
#include <MessageQueue.hpp>


int main()
{
    FindIt::MessageQueue *queue = new FindIt::MessageQueue();
    FindIt::HeartBeatResponse message("node_name", 1);
    FindIt::HeartBeatResponse *message2;

    queue->push(&message);
    message2 = (FindIt::HeartBeatResponse*)queue->pop();

    std::string lol = message2->GetNode();
    std::cout << lol << std::endl;

    while(true)
    {
        //so the application doesnt close
    }

    return 0;
}
