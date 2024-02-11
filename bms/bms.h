#ifndef BMS_H
#define BMS_H

#include <plugins/pluginload/plugininterface.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <memory>

class Tag;
class QTimer;
class TagList;

class Bms : public PluginInterface
{
    Q_OBJECT
public:
    Bms();
    void setTagSystem(TagList *taglist) override;

    bool initialize() override;
    void run(int deltaMs) override;
    void stop() override;

protected slots:
    void mainloop();
    void onReply(QNetworkReply *reply);

private:
    void setupTags();
    TagList *tagList_ = nullptr;
    QNetworkAccessManager manager_;

    QTimer* mainLoopTimer_ = nullptr;

    Tag* voltageTag_ = nullptr;
    Tag* cell1_ = nullptr;
    Tag* cell2_ = nullptr;
    Tag* cell3_ = nullptr;
    Tag* cell4_ = nullptr;
    Tag* cell5_ = nullptr;
    Tag* cell6_ = nullptr;
    Tag* cell7_ = nullptr;
    Tag* cell8_ = nullptr;

    Tag* trimmer0_ = nullptr;
    Tag* trimmer1_ = nullptr;

    Tag* cellMinSet_ = nullptr;
    Tag* cellMaxSet_ = nullptr;

    Tag* uMinCell_ = nullptr;
    Tag* uMaxCell_ = nullptr;

    Tag* uMinCellId_ = nullptr;
    Tag* uMaxCellId_ = nullptr;
    Tag* uDiff_ = nullptr;

/*    <lastcellevent>N/A</lastcellevent>
    <lastcellno>N/A</lastcellno>
    <lastcellU>N/A</lastcellU>
    <lastbattU>N/A</lastbattU>
    <connection>CONNECTED</connection>
    <status>0xFFFF</status>
    <relay1>NORMAL</relay1>
    <relay2>NORMAL</relay2> */
};


#endif // BMS_H
