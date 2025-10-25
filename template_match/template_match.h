#ifndef TEMPLATE_MATCH_H
#define TEMPLATE_MATCH_H

#include <plugins/plugincore/plugin.h>

#include <QObject>
#include <QImage>

class Tag;

namespace plugin {

class TemplateMatch : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.templatematch")
public:
    TemplateMatch() = default;

    bool initialize() final;

private slots:
    void mainloop();

private:
    bool isFound_ = false;
    QImage template_;
    Tag* foundTag_ = nullptr;
};


} // end namespace

#endif
