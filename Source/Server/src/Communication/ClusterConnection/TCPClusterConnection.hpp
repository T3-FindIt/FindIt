#ifndef TCPCLUSTERCONNECTION_HPP
#define TCPCLUSTERCONNECTION_HPP

#include "IClusterConnection.hpp"
#include <string>

namespace FindIt
{
    using Event_function_t = void();
    
    class TCPClusterConnection : public IClusterConnection
    {
        private:
        int port;
        std::string myip;
        public:
        [[noreturn]] void Run();
        void Stop();
        void Broadcast(iMessage message);
        void SetOnMessagehandler(Event_function_t handler);
    };
};

#endif //ICLUSTERCONNECTION_HPP
