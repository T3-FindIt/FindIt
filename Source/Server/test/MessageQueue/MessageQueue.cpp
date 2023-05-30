#include <gtest/gtest.h>
#include <IMessage.hpp>

#include "MessageQueue.hpp"
#include <thread>

#include <HeartbeatResponse.hpp>
#include <NodeEventProductFound.hpp>
#include <NodeNotifyNewProduct.hpp>
#include <NodeRespondToProductRequest.hpp>
#include <NodeSignIn.hpp>

#include <HeartBeat.hpp>
#include <NodeNotifyNewProductResponse.hpp>
#include <NodeSignInResponse.hpp>
#include <ServerRequestProduct.hpp>

namespace
{
    TEST(MessageQueueTests, queue_push_Test)
    {
        FindIt::MessageQueue queue;
        FindIt::HeartBeatResponse message1("node_name 1", 1);
        FindIt::NodeNotifyNewProduct message2("product_name");
        FindIt::ServerRequestProduct message3("chocola jummie", true);

        queue.push(&message1);
        queue.push(&message2);
        queue.push(&message3);

        FindIt::HeartBeatResponse *result = (FindIt::HeartBeatResponse *)queue.pop();
        EXPECT_EQ(result->GetNode(), "node_name 1");
        FindIt::NodeNotifyNewProduct *result2 = (FindIt::NodeNotifyNewProduct *)queue.pop();
        EXPECT_EQ(result2->GetProduct(), "product_name");
        FindIt::ServerRequestProduct *result3 = (FindIt::ServerRequestProduct *)queue.pop();
        EXPECT_EQ(result3->GetProduct(), "chocola jummie");
    }

    TEST(MessageQueueTests, queue_pop_null_Test)
    {
        FindIt::MessageQueue queue;

        EXPECT_EQ(queue.pop(), nullptr);
    }

    TEST(MessageQueueTests, queue_size_Test)
    {
        FindIt::MessageQueue queue;

        FindIt::HeartBeatResponse message("node_name", 1);
        queue.push(&message);
        queue.push(&message);
        queue.push(&message);
        queue.push(&message);

        EXPECT_EQ(queue.returnSize(), 4);
    }

    void spam(FindIt::MessageQueue &queue)
    {
        FindIt::HeartBeatResponse *message = new FindIt::HeartBeatResponse("node_name", 1);
        for (int i = 0; i < 50; i++)
        {
            queue.push(message);
        }
    }

    TEST(MessageQueueTests, queue_thread_safety_Test)
    {
        FindIt::MessageQueue queue;

        std::jthread t1(spam, std::ref(queue));
        std::jthread t2(spam, std::ref(queue));
        std::jthread t3(spam, std::ref(queue));

        t1.join();
        t2.join();
        t3.join();

        FindIt::HeartBeatResponse *result;

        for (int i = 0; i < 149; i++)
        {
            result = (FindIt::HeartBeatResponse *)queue.pop();
            EXPECT_EQ(result->GetNode(), "node_name");
            result->~HeartBeatResponse();
        }
    }

} // namespace
