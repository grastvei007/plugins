#ifndef PLUGININTERFACE
#define PLUGININTERFACE

#include <vector>
#include <string>

class TagList;

class PluginInterface
{
public:
    virtual void setTagSystem(TagList *taglist) = 0;
    virtual bool initialize() = 0;
    virtual void run(int deltaMs) = 0;
    virtual void stop() = 0;
};

#endif
