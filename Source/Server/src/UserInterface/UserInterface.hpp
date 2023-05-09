#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <string>
#include <vector>

namespace FindIt
{

using Get_objects_t = std::vector<std::string>();

class Userinterface
{
private:
    
public:
    Userinterface(); //needed?
    ~Userinterface();

    void Run();
    void Stop();
};

};

#endif // COMMUNICATION_HPP
