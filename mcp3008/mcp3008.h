#ifndef MCP_3008_H
#define MCP_3008_H

#include <plugins/pluginload/plugininterface.h>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

#include <QObject>
#include <QTimer>
#include <QVector>

class Mcp3008: public QObject, public PluginInterface
{
    Q_OBJECT
public:
    Mcp3008();

    void setTagSystem(TagList *taglist) override;
    bool initialize() override;
    void run(int deltaMs) override;
    void stop() override;

private slots:
    void mainloop();

private:
    QTimer* mainLoopTimer_ = nullptr;
    TagList *tagList_ = nullptr;

    int spiChan_ = 0;
    int lowestPin_ = 100;
    double constant = 3.3/1024.; // 3.3v divided by max value 8 bit adc

    QVector<Tag*> channel_;

};

#endif
