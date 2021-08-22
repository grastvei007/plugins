#ifndef BMS_H
#define BMS_H

#include <plugins/include/plugininterface.h>

#include <tagsystem/tag.h>

class Bms : public PluginInterface
{
public:
    Bms();

    bool initialize(std::vector<std::string> &args) override;
    void run() override;
    void stop() override;

private:
    Tag *voltageTag;
};

#endif // BMS_H
