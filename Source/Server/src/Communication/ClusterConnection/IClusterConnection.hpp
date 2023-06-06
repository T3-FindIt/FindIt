#ifndef ICLUSTERCONNECTION_HPP
#define ICLUSTERCONNECTION_HPP

#include <functional>
#include <cstdint>
#include <string>

namespace FindIt
{

using on_client_message_callback_t     = std::function<void(const uint64_t, const std::string&)>;
using on_client_connect_callback_t     = std::function<void(const uint64_t)>;
using on_client_disconnect_callback_t  = std::function<void(const uint64_t)>;

class IClusterConnection
{
protected:
    bool m_running;

    on_client_message_callback_t    m_on_client_message;
    on_client_connect_callback_t    m_on_client_connect;
    on_client_disconnect_callback_t m_on_client_disconnect;
public:
    virtual ~IClusterConnection() = default;
    virtual void run() = 0;
    virtual void stop() = 0;

    virtual void setCallbacks(on_client_message_callback_t on_client_message,
                            on_client_connect_callback_t on_client_connect,
                            on_client_disconnect_callback_t on_client_disconnect) = 0;

    virtual void broadcastMessage(const std::string& message) = 0;

    virtual void closeClient(const uint64_t client) = 0;
};

};

#endif // ICLUSTERCONNECTION_HPP
