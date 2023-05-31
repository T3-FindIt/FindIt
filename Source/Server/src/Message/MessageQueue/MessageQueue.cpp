#include "MessageQueue.hpp"

namespace FindIt
{
    void MessageQueue::push(IMessage *message)
    {
        std::scoped_lock lock(mutex);
        queue.push(message);
    }

    IMessage *MessageQueue::pop() //Raw pointer? Why?
    {
        std::scoped_lock lock(mutex);
        if (queue.size() <= 0) //bad?
        {
            return nullptr;
        }

        IMessage *message = queue.front();
        queue.pop();
        return message;
    }

    int MessageQueue::returnSize()
    {
        std::scoped_lock lock(mutex);
        return queue.size();
    }
}; // namespace FindIt
