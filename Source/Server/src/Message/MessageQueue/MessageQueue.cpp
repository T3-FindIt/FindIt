#include "MessageQueue.hpp"

namespace FindIt
{

    MessageQueue::MessageQueue()
    {
    }

    MessageQueue::~MessageQueue()
    {
    }

    void MessageQueue::push(IMessage *message)
    {
        queue.push(message);
    }

    IMessage *MessageQueue::pop()
    {
        IMessage *message = queue.front();
        queue.pop();
        return message;
    }
}; // namespace FindIt
