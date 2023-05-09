#ifndef FAILEDWSASTARTUP_HPP
#define FAILEDWSASTARTUP_HPP

#include <exception>
#include <string>

class FailedWSAStartup : public std::exception
{
public:
    explicit FailedWSAStartup(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return this->m_message.c_str(); }
private:
    std::string m_message;
};

#endif // FAILEDWSASTARTUP_HPP
