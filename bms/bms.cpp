#include "bms.h"

#include <QTimer>
#include <QXmlStreamReader>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

namespace plugin {


bool Bms::initialize()
{
    connect(&manager_, &QNetworkAccessManager::finished, this, &Bms::onReply);

    setupTags();
    return true;
}

void Bms::loop()
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
            if(stream.name() == QString("data"))
                continue;
            if(stream.name() == QString("utotal"))
            {
                double value = stream.readElementText().toDouble();
                if(voltageTag_)
                    voltageTag_->setValue(value);
                else
                {
                    voltageTag_ = tagList()->createTag("bms", "voltage", TagType::eDouble, value);
                }
            }
            else if(stream.name() == QString("ucell1")) //>3.28</ucell1>
            {
                auto value = stream.readElementText();
                if(value == QString("N/A"))
                    continue;

                double v = value.toDouble();
                if(cell1_)
                    cell1_->setValue(v);
                else
                {
                    cell1_ = tagList()->createTag("bms", "cell 1", TagType::eDouble, v);
                }
            }
            else if(stream.name() == QString("ucell2")) //>3.29</ucell2>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell2_)
                    cell2_->setValue(v);
                else
                {
                    cell2_ = tagList()->createTag("bms", "cell 2", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell3")) // >3.29</ucell3>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell3_)
                    cell3_->setValue(v);
                else
                {
                    cell3_ = tagList()->createTag("bms", "cell 3", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell4")) // >3.28</ucell4>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell4_)
                    cell4_->setValue(v);
                else
                {
                    cell4_ = tagList()->createTag("bms", "cell 4", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell5")) // >N/A</ucell5>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell5_)
                    cell5_->setValue(v);
                else
                {
                    cell5_ = tagList()->createTag("bms", "cell 5", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell6")) //>N/A</ucell6>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell6_)
                    cell6_->setValue(v);
                else
                {
                    cell6_ = tagList()->createTag("bms", "cell 6", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell7")) //>N/A</ucell7>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell7_)
                    cell7_->setValue(v);
                else
                {
                    cell7_ = tagList()->createTag("bms", "cell 7", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell8")) //>N/A</ucell8>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(cell8_)
                    cell8_->setValue(v);
                else
                {
                    cell8_ = tagList()->createTag("bms", "cell 8", TagType::eDouble, v);
                }

            }
            else if(stream.name() == QString("ucell9")) //>N/A</ucell9>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell10")) //>N/A</ucell10>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell11")) //>N/A</ucell11>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell12")) //>N/A</ucell12>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell13")) // >N/A</ucell13>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell14")) //>N/A</ucell14>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell15")) //>N/A</ucell15>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("ucell16")) //>N/A</ucell16>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;

            }
            else if(stream.name() == QString("trimmer-0")) //>2.80</trimmer-0>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(trimmer0_)
                    trimmer0_->setValue(v);
                else
                    trimmer0_ = tagList()->createTag("bms", "trimmer 0", TagType::eDouble, v);

            }
            else if(stream.name() == QString("trimmer-1")) // >3.60</trimmer-1>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(trimmer1_)
                    trimmer1_->setValue(v);
                else
                    trimmer1_ = tagList()->createTag("bms", "trimmer 1", TagType::eDouble, v);

            }
            else if(stream.name() == QString("UMINset")) //>2800</UMINset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uMinCell_)
                    uMinCell_->setValue(v);
                else
                    uMinCell_ = tagList()->createTag("bms", "uMinCell", TagType::eDouble, v);
            }
            else if(stream.name() == QString("UMAXset")) //>3600</UMAXset>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uMaxCell_)
                    uMaxCell_->setValue(v);
                else
                    uMaxCell_ = tagList()->createTag("bms", "uMaxCell", TagType::eDouble, v);

            }
            else if(stream.name() == QString("Umincell")) //>3.28</Umincell>
            {


            }
            else if(stream.name() == QString("Umaxcell")) //>3.29</Umaxcell>
            {


            }
            else if(stream.name() == QString("UmincellID")) //>1</UmincellID>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                int v = value.toInt();
                if(uMinCellId_)
                    uMinCellId_->setValue(v);
                else
                    uMinCellId_ = tagList()->createTag("bms", "min voltage cell id", TagType::eInt, v);
            }
            else if(stream.name() == QString("UmaxcellID")) //>3</UmaxcellID>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                int v = value.toInt();
                if(uMaxCellId_)
                    uMaxCellId_->setValue(v);
                else
                    uMaxCellId_ = tagList()->createTag("bms", "max voltage cell id", TagType::eInt);
            }
            else if(stream.name() == QString("Udiff")) //>0.01</Udiff>
            {
                auto value = stream.readElementText();
                if(value == "N/A")
                    continue;
                double v = value.toDouble();
                if(uDiff_)
                    uDiff_->setValue(v);
                else
                    uDiff_ = tagList()->createTag("bms", "difference min max cell", TagType::eDouble, v);
            }
            else if(stream.name() == QString("lastcellevent")) //>N/A</lastcellevent>
            {

            }
            else if(stream.name() == QString("lastcellno")) //>N/A</lastcellno>
            {

            }
            else if(stream.name() == QString("lastcellU")) // >N/A</lastcellU>
            {

            }
            else if(stream.name() == QString("lastbattU")) //>N/A</lastbattU>
            {

            }
            else if(stream.name() == QString("connection")) //>CONNECTED</connection>
            {

            }
            else if(stream.name() == QString("status")) //>0xFFFF</status>
            {

            }
            else if(stream.name() == QString("relay1")) //>NORMAL</relay1>
            {

            }
            else if(stream.name() == QString("relay2")) //>NORMAL</relay2>
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
    cellMinSet_ = tagList()->createTag("bms", "configUMinCell", TagType::eDouble);
    cellMaxSet_ = tagList()->createTag("bms", "configUMaxCell", TagType::eDouble);
}


} // end namespace
