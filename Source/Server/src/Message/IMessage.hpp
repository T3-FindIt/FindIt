#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

namespace FindIt
{

enum class MessageType
{
    SERVER_REQUEST_PRODUCT,
    NODE_RESPOND_TO_PRODUCT_REQUEST,
    NODE_EVENT_PRODUCT_FOUND,
    NODE_SIGN_IN,
    NODE_SIGN_IN_RESPONSE,
    NODE_NOTIFY_NEW_PRODUCT,
    NODE_NOTIFY_NEW_PRODUCT_RESPONSE,
    HEARTBEAT,
    HEARTBEAT_RESPONSE,
};

class IMessage
{
protected:
    MessageType type;
    std::string action;
public:
    virtual MessageType GetType() = 0;
};

};

#endif // IMESSAGE_HPP
