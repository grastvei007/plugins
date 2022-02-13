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
                if(voltageTag_)
                    voltageTag_->setValue(value);
                else
                {
                    voltageTag_ = tagList_->createTag("bms", "voltage", Tag::eDouble, value);
                }
            }
            else if(stream.name() == "ucell1") //>3.28</ucell1>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

                double v = value.toDouble();
                if(cell1_)
                    cell1_->setValue(v);
                else
                {
                    cell1_ = tagList_->createTag("bms", "cell 1", Tag::eDouble, v);
                }
            }
            else if(stream.name() == "ucell2") //>3.29</ucell2>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell2_)
                    cell2_->setValue(v);
                else
                {
                    cell2_ = tagList_->createTag("bms", "cell 2", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell3") // >3.29</ucell3>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell3_)
                    cell3_->setValue(v);
                else
                {
                    cell3_ = tagList_->createTag("bms", "cell 3", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell4") // >3.28</ucell4>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell4_)
                    cell4_->setValue(v);
                else
                {
                    cell4_ = tagList_->createTag("bms", "cell 4", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell5") // >N/A</ucell5>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell5_)
                    cell5_->setValue(v);
                else
                {
                    cell5_ = tagList_->createTag("bms", "cell 5", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell6") //>N/A</ucell6>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell6_)
                    cell6_->setValue(v);
                else
                {
                    cell6_ = tagList_->createTag("bms", "cell 6", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell7") //>N/A</ucell7>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell7_)
                    cell7_->setValue(v);
                else
                {
                    cell7_ = tagList_->createTag("bms", "cell 7", Tag::eDouble, v);
                }

            }
            else if(stream.name() == "ucell8") //>N/A</ucell8>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell8_)
                    cell8_->setValue(v);
                else
                {
                    cell8_ = tagList_->createTag("bms", "cell 8", Tag::eDouble, v);
                }

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
                if(trimmer0_)
                    trimmer0_->setValue(v);
                else
                    trimmer0_ = tagList_->createTag("bms", "trimmer 0", Tag::eDouble, v);

            }
            else if(stream.name() == "trimmer-1") // >3.60</trimmer-1>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(trimmer1_)
                    trimmer1_->setValue(v);
                else
                    trimmer1_ = tagList_->createTag("bms", "trimmer 1", Tag::eDouble, v);

            }
            else if(stream.name() == "UMINset") //>2800</UMINset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uMinCell_)
                    uMinCell_->setValue(v);
                else
                    uMinCell_ = tagList_->createTag("bms", "uMinCell", Tag::eDouble, v);
            }
            else if(stream.name() == "UMAXset") //>3600</UMAXset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uMaxCell_)
                    uMaxCell_->setValue(v);
                else
                    uMaxCell_ = tagList_->createTag("bms", "uMaxCell", Tag::eDouble, v);

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
                if(uMinCellId_)
                    uMinCellId_->setValue(v);
                else
                    uMinCellId_ = tagList_->createTag("bms", "min voltage cell id", Tag::eInt, v);
            }
            else if(stream.name() == "UmaxcellID") //>3</UmaxcellID>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                int v = value.toInt();
                if(uMaxCellId_)
                    uMaxCellId_->setValue(v);
                else
                    uMaxCellId_ = tagList_->createTag("bms", "max voltage cell id", Tag::eInt);
            }
            else if(stream.name() == "Udiff") //>0.01</Udiff>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uDiff_)
                    uDiff_->setValue(v);
                else
                    uDiff_ = tagList_->createTag("bms", "difference min max cell", Tag::eDouble, v);
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




    cellMinSet_ = tagList_->createTag("bms", "configUMinCell", Tag::eDouble);
    cellMaxSet_ = tagList_->createTag("bms", "configUMaxCell", Tag::eDouble);


}

extern "C" PluginInterface* createPlugin()
{
    return new Bms();
}
