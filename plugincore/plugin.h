#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugins/pluginload/plugininterface.h"

#include <QObject>
#include <QTimer>

#include <memory>


class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
public:
    Plugin() = default;

    void setTagSystem(TagList *taglist) override;

    virtual bool initialize() override;
    void run(int deltaMs) override;
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
