#ifndef DS18B20_H
#define DS18B20_H

#include <plugins/plugincore/plugin.h>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

#include <map>

namespace plugin{

class Ds18b20 : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.ds18b20")
public:
    Ds18b20();

    bool initialize() final;

private slots:
    void mainloop();

private:
    double readSensorValue(const QString &filePath);
    QTimer* mainLoopTimer_ = nullptr;
    TagList *tagList_ = nullptr;
    std::map<QString, Tag*> temperatureSensors_;
    int readSensor_ = -1;

    std::map<QString, QString> folderToName_;
};

}//end namespace

#endif
