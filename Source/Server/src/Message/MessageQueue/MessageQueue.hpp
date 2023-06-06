#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

//unused includes
#include <IMessage.hpp>
#include <HeartBeat.hpp>
#include <mutex>
#include <queue>
#include <memory>

namespace FindIt
{
    class MessageQueue
    {
    public:
        MessageQueue() = default;
        ~MessageQueue() = default;

        void push(std::shared_ptr<IMessage> message);
        std::shared_ptr<IMessage> pop();
        int returnSize();

    private:
        std::queue<std::shared_ptr<IMessage>> queue;
        std::mutex mutex;
    };

}; // namespace FindIt

#endif // MESSAGEQUEUE_HPP
