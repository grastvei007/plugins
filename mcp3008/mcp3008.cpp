#include "mcp3008.h"
#include "wiringpiwrapper.h"

extern "C" PluginInterface* createPlugin()
{
  WiringPi::setup();
  return new Mcp3008();
}


Mcp3008::Mcp3008()
{

}

void Mcp3008::setTagSystem(TagList *taglist)
{
    tagList_ = taglist;
}

bool Mcp3008::initialize()
{
    WiringPi::mcp3008Setup(lowestPin_, spiChan_);

    channel_.push_back(tagList_->createTag("mcp3008", "ch0", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch1", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch2", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch3", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch4", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch5", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch6", Tag::eDouble));
    channel_.push_back(tagList_->createTag("mcp3008", "ch7", Tag::eDouble));

    for(int i=0; i<8; ++i)
        channel_[i]->setValue(0.0);

    return true;
}

void Mcp3008::run(int deltaMs)
{
    if(mainLoopTimer_)
        mainLoopTimer_->deleteLater();

    mainLoopTimer_ = new QTimer();
    mainLoopTimer_->setInterval(deltaMs);
    QObject::connect(mainLoopTimer_, &QTimer::timeout, this, &Mcp3008::mainloop);

    mainLoopTimer_->start();
}

void Mcp3008::stop()
{

}

void Mcp3008::mainloop()
{
    for(int i=0; i<8; i++)
    {
        int adc = WiringPi::analogRead(lowestPin_ + i);
        double voltage = constant * adc;
        channel_[i]->setValue(voltage);
    }
}
