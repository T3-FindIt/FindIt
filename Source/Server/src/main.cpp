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
    FindIt::HeartBeatResponse *message0;
    std::string lol;

    FindIt::HeartBeatResponse message("node_name 1", 1);
    queue->push(&message);

    FindIt::HeartBeatResponse message2("node_name 2", 1);
    queue->push(&message2);

    FindIt::HeartBeatResponse message3("node_name 3", 1);
    queue->push(&message3);

    FindIt::HeartBeatResponse message4("node_name 4", 1);
    queue->push(&message4);

    FindIt::HeartBeatResponse message5("node_name 5", 1);
    queue->push(&message5);

    for (int i = 0; i < 5; i++)
    {
        message0 = (FindIt::HeartBeatResponse *)queue->pop();
        lol = message0->GetNode();
        std::cout << lol << std::endl;
    }

    while (true)
    {
        // so the application doesnt close
    }

    return 0;
}
