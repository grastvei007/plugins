#ifndef BMS_H
#define BMS_H

#include <plugins/plugincore/plugin.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class Tag;
class QTimer;
class TagList;

namespace plugin {

class Bms : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "june.plugin.bms")
public:
    Bms() = default;

    bool initialize() override;

private slots:
    void mainloop() final;
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

}// end namespace
#endif // BMS_H
