#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include <IMessage.hpp>
#include <HeartBeat.hpp>
#include <mutex>
#include <queue>

namespace FindIt
{
    class MessageQueue
    {
    public:
        MessageQueue();
        ~MessageQueue();

        void push(IMessage *message);
        IMessage *pop();

    private:
        std::queue<IMessage*> queue;
        std::mutex mutex;
    };

}; // namespace FindIt

#endif // MESSAGEQUEUE_HPP
