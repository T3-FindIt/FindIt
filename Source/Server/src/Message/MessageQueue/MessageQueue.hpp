#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

//Unused includes
#include <IMessage.hpp>
#include <HeartBeat.hpp>
#include <mutex>
#include <queue>

namespace FindIt
{
    //Looks like a wrapper around std::queue with a mutex. Nothing wrong with that, looks good.
    //Lose the raw pointers though.
    //Extra points: Try to see if you can make the implementation generic, like std::queue (only if you want to torture yourself a little)
    class MessageQueue
    {
    public:
        MessageQueue() = default; //Nice
        ~MessageQueue() = default; //Nice

        void push(IMessage *message);
        IMessage *pop();
        int returnSize();

    private:
        std::queue<IMessage*> queue; //No need for the use of raw pointers here. You can use references instead.
        std::mutex mutex;
    };

}; // namespace FindIt

#endif // MESSAGEQUEUE_HPP
