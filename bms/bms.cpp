#include "bms.h"

#include <QTimer>
#include <QXmlStreamReader>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>


Bms::Bms()
{

}

void Bms::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool Bms::initialize()
{
    connect(&manager_, &QNetworkAccessManager::finished, this, &Bms::onReply);

    if(!tagList_)
    {
        tagList_ = &TagList::sGetInstance();
        tagList_->connectToServer("localhost", 5000);
        tagList_->setClientName("bms");
    }
    setupTags();
    mainloop();
    return true;
}

void Bms::run(int deltaMs)
{
    if(!mainLoopTimer_)
        mainLoopTimer_->deleteLater();

    mainLoopTimer_ = new QTimer();
    mainLoopTimer_->setInterval(deltaMs);
    QObject::connect(mainLoopTimer_, &QTimer::timeout, this, &Bms::mainloop);

    mainLoopTimer_->start();
}

void Bms::stop()
{
    mainLoopTimer_->stop();
}

void Bms::mainloop()
{

    manager_.get(QNetworkRequest(QUrl("http://192.168.0.100/bcc.xml")));
  //  qDebug() << __FUNCTION__;
}

void Bms::onReply(QNetworkReply *reply)
{
    auto xmlDocument = reply->readAll();
    QXmlStreamReader stream(xmlDocument);

    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == "data")
                continue;
            if(stream.name() == "utotal")
            {
                double value = stream.readElementText().toDouble();
                voltageTag_->setValue(value);
                //qDebug() << stream.name() << " - " << value;
            }
            else if(stream.name() == "ucell1") //>3.28</ucell1>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

                double v = value.toDouble();
                cell1_->setValue(v);
            }
            else if(stream.name() == "ucell2") //>3.29</ucell2>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                cell2_->setValue(v);

            }
            else if(stream.name() == "ucell3") // >3.29</ucell3>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                cell3_->setValue(v);

            }
            else if(stream.name() == "ucell4") // >3.28</ucell4>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                cell4_->setValue(v);

            }
            else if(stream.name() == "ucell5") // >N/A</ucell5>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell6") //>N/A</ucell6>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell7") //>N/A</ucell7>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell8") //>N/A</ucell8>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell9") //>N/A</ucell9>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell10") //>N/A</ucell10>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell11") //>N/A</ucell11>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell12") //>N/A</ucell12>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell13") // >N/A</ucell13>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell14") //>N/A</ucell14>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell15") //>N/A</ucell15>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "ucell16") //>N/A</ucell16>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == "trimmer-0") //>2.80</trimmer-0>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                trimmer0_->setValue(v);

            }
            else if(stream.name() == "trimmer-1") // >3.60</trimmer-1>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                trimmer1_->setValue(v);

            }
            else if(stream.name() == "UMINset") //>2800</UMINset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                uMinCell_->setValue(v);
            }
            else if(stream.name() == "UMAXset") //>3600</UMAXset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                uMaxCell_->setValue(v);

            }
            else if(stream.name() == "Umincell") //>3.28</Umincell>
            {


            }
            else if(stream.name() == "Umaxcell") //>3.29</Umaxcell>
            {


            }
            else if(stream.name() == "UmincellID") //>1</UmincellID>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                int v = value.toInt();
                uMinCellId_->setValue(v);
            }
            else if(stream.name() == "UmaxcellID") //>3</UmaxcellID>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                int v = value.toInt();
                uMaxCellId_->setValue(v);
            }
            else if(stream.name() == "Udiff") //>0.01</Udiff>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                uDiff_->setValue(v);
            }
            else if(stream.name() == "lastcellevent") //>N/A</lastcellevent>
            {

            }
            else if(stream.name() == "lastcellno") //>N/A</lastcellno>
            {

            }
            else if(stream.name() == "lastcellU") // >N/A</lastcellU>
            {

            }
            else if(stream.name() == "lastbattU") //>N/A</lastbattU>
            {

            }
            else if(stream.name() == "connection") //>CONNECTED</connection>
            {

            }
            else if(stream.name() == "status") //>0xFFFF</status>
            {

            }
            else if(stream.name() == "relay1") //>NORMAL</relay1>
            {

            }
            else if(stream.name() == "relay2") //>NORMAL</relay2>
            {

            }
            //qDebug() << stream.name() << " - "  << stream.readElementText();

        }
    }
    if(stream.hasError())
    {
        qDebug() << __FUNCTION__ << stream.errorString();
    }
}

void Bms::setupTags()
{
    voltageTag_ = tagList_->createTag("bms", "voltage", Tag::eDouble);
    cell1_ = tagList_->createTag("bms", "cell 1", Tag::eDouble);
    cell2_ = tagList_->createTag("bms", "cell 2", Tag::eDouble);
    cell3_ = tagList_->createTag("bms", "cell 3", Tag::eDouble);
    cell4_ = tagList_->createTag("bms", "cell 4", Tag::eDouble);

    trimmer0_ = tagList_->createTag("bms", "trimmer 0", Tag::eDouble);
    trimmer1_ = tagList_->createTag("bms", "trimmer 1", Tag::eDouble);

    cellMinSet_ = tagList_->createTag("bms", "configUMinCell", Tag::eDouble);
    cellMaxSet_ = tagList_->createTag("bms", "configUMaxCell", Tag::eDouble);

    uMinCell_ = tagList_->createTag("bms", "uMinCell", Tag::eDouble);
    uMaxCell_ = tagList_->createTag("bms", "uMaxCell", Tag::eDouble);

    uMinCellId_ = tagList_->createTag("bms", "min voltage cell id", Tag::eInt);
    uMaxCellId_ = tagList_->createTag("bms", "max voltage cell id", Tag::eInt);
    uDiff_ = tagList_->createTag("bms", "difference min max cell", Tag::eDouble);
}

extern "C" PluginInterface* createPlugin()
{
    return new Bms();
}
