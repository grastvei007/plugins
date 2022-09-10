#ifndef DS18B20_H
#define DS18B20_H

#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

#include <map>


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
    double readSensorValue(const QString &filePath);
    QTimer* mainLoopTimer_ = nullptr;
    TagList *tagList_ = nullptr;
    std::map<QString, Tag*> temperatureSensors_;
    int readSensor_ = -1;

    std::map<QString, QString> folderToName_;
};



#endif
