#ifndef DS18B20_H
#define DS18B20_H

#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>


class Ds18b20 : public QObject, public PluginInterface
{
    Q_OBJECT
public:
    Ds18b20();

    void setTagSystem(TagList *taglist) override;
    bool initialize() override;
    void run(int deltaMs) override;
    void stop() override;

private slots:
    void mainloop();

private:
    QTimer* mainLoopTimer_ = nullptr;
    TagList *tagList_ = nullptr;
};



#endif
