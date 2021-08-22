#include <vector>
#include <string>

class PluginInterface
{
    virtual bool initialize(std::vector<std::string> &args) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};
