#include "MessageQueue.hpp"

namespace FindIt
{
    void MessageQueue::push(std::shared_ptr<IMessage> message)
    {
        std::scoped_lock lock(mutex);
        queue.push(message);
    }

    std::shared_ptr<IMessage> MessageQueue::pop()
    {
        std::scoped_lock lock(mutex);
        if (queue.size() <= 0)
        {
            return nullptr;
        }

        std::shared_ptr<IMessage> message = queue.front();
        queue.pop();
        return message;
    }

    int MessageQueue::returnSize()
    {
        std::scoped_lock lock(mutex);
        return queue.size();
    }
}; // namespace FindIt
