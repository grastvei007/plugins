#ifndef PIGPIO_H
#define PIGPIO_H

#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

#include <QObject>
#include <QTimer>
#include <QVector>

class Pin;


class PiGpio :  public QObject, public PluginInterface
{
public:
    PiGpio();

    void setTagSystem(TagList *taglist) override;
    bool initialize() override;
    void run(int deltaMs) override;
    void stop() override;

private slots:
    void mainloop();

private:
    QTimer* mainLoopTimer_ = nullptr;
    int deltaMs_ = 0;
    TagList *tagList_ = nullptr;
    QVector<Pin*> pins_;
};

#endif // PIGPIO_H
