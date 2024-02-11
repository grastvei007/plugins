#ifndef PLUGININTERFACE
#define PLUGININTERFACE

#include <vector>
#include <string>
#include <QObject>
/**
 * To create a plugin inherit the pluginInterface,
 * and put the folowing in the new library
 *
 * extern "C" PluginInterface* createPlugin()
 * {
 *   return new MyNewPlugin(); // return pointer to the new plugin
 * }
 *
 *
 */

class TagList;

class PluginInterface : public QObject
{
    Q_OBJECT
public:
    virtual void setTagSystem(TagList *taglist) = 0;
    virtual bool initialize() = 0;
    virtual void run(int deltaMs) = 0;
    virtual void stop() = 0;
};

#endif
