#ifndef PLUGININTERFACE
#define PLUGININTERFACE

#include <vector>
#include <string>
#include <QObject>
/**
 * Public plugin interface known to applications that
 * uses plugins.
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
