#include <gtest/gtest.h>
#include <IMessage.hpp>

#include "MessageQueue.hpp"
#include <thread>
#include <memory>

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

        std::shared_ptr<FindIt::IMessage> msg = std::make_shared<FindIt::HeartBeatResponse>("node_name 1", 1);
        std::shared_ptr<FindIt::IMessage> msg2 = std::make_shared<FindIt::NodeNotifyNewProduct>("product_name");
        std::shared_ptr<FindIt::IMessage> msg3 = std::make_shared<FindIt::ServerRequestProduct>("chocola jummie", true);
        queue.push(msg);
        queue.push(msg2);
        queue.push(msg3);

        std::shared_ptr<FindIt::IMessage> result1 = queue.pop();
        FindIt::HeartBeatResponse *result11 = (FindIt::HeartBeatResponse *)result1.get();
        EXPECT_EQ(result11->GetNode(), "node_name 1");

        std::shared_ptr<FindIt::IMessage> result2 = queue.pop();
        FindIt::NodeNotifyNewProduct *result21 = (FindIt::NodeNotifyNewProduct *)result2.get();
        EXPECT_EQ(result21->GetProduct(), "product_name");

        std::shared_ptr<FindIt::IMessage> result3 = queue.pop();
        FindIt::ServerRequestProduct *result31 = (FindIt::ServerRequestProduct *)result3.get();
        EXPECT_EQ(result31->GetProduct(), "chocola jummie");
    }

    TEST(MessageQueueTests, queue_pop_null_Test)
    {
        FindIt::MessageQueue queue;

        EXPECT_EQ(queue.pop(), nullptr);
    }

    TEST(MessageQueueTests, queue_size_Test)
    {
        FindIt::MessageQueue queue;

        std::shared_ptr<FindIt::IMessage> msg = std::make_shared<FindIt::HeartBeatResponse>("node_name 1", 1);
        queue.push(msg);
        queue.push(msg);
        queue.push(msg);
        queue.push(msg);

        EXPECT_EQ(queue.returnSize(), 4);
    }

    void spam(FindIt::MessageQueue &queue)
    {
        std::shared_ptr<FindIt::IMessage> msg = std::make_shared<FindIt::HeartBeatResponse>("node_name 1", 1);
        for (int i = 0; i < 50; i++)
        {
            queue.push(msg);
        }
    }

    TEST(MessageQueueTests, queue_thread_safety_Test)
    {
        FindIt::MessageQueue queue;

        std::thread t1(spam, std::ref(queue));
        std::thread t2(spam, std::ref(queue));
        std::thread t3(spam, std::ref(queue));

        t1.join();
        t2.join();
        t3.join();

        std::shared_ptr<FindIt::IMessage> result1;
        FindIt::HeartBeatResponse *result11;

        for (int i = 0; i < 149; i++)
        {
            result1 = queue.pop();
            result11 = (FindIt::HeartBeatResponse *)result1.get();
            EXPECT_EQ(result11->GetNode(), "node_name 1");
        }
    }

} // namespace
