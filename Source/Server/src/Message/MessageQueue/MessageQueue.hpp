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
        std::queue<std::shared_ptr<IMessage>> queue; //No need for the use of raw pointers here. You can use references instead.
        std::mutex mutex;
    };

}; // namespace FindIt

#endif // MESSAGEQUEUE_HPP
