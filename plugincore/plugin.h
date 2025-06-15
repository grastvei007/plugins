#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugins/pluginload/plugininterface.h"

#include <QObject>
#include <QTimer>

#include <memory>

/**
 * @brief The Plugin class
 *
 * Plugin base class. To implement a plugin override
 * - initialize to setup the plugin tags
 * - mainloop with the plugin logic
 */
class Plugin : public PluginInterface
{
    Q_OBJECT
public:
    Plugin() = default;

    void setTagSystem(TagList *taglist) override;
    // optional, override if needed.
    // it should be called when plugins are load, the
    // default is a falltrough.
    void createApi(QHttpServer &httpserver) override;

    virtual bool initialize() override;
    void run(int deltaMs = 1000) override;
    void stop() override;

protected:
    TagList* tagList() const;

protected slots:
    virtual void mainloop();

private:
    TagList *tagList_ = nullptr;
    std::unique_ptr<QTimer> mainLoopTimer_;
};

#endif // PLUGIN_H
