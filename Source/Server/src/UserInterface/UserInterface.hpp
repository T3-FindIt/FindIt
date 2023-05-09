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
    Userinterface();
    ~Userinterface();

    void Run();
    void Stop();
};

};

#endif // USERINTERFACE_HPP
