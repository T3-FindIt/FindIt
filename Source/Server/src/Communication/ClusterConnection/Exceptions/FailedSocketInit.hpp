#ifndef FAILEDSOCKETINIT_HPP
#define FAILEDSOCKETINIT_HPP

#include <exception>
#include <string>

class FailedSocketInit : public std::exception
{
public:
    explicit FailedSocketInit(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return this->m_message.c_str(); }
private:
    std::string m_message;
};

#endif // FAILEDSOCKETINIT_HPP
