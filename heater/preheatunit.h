#ifndef PREHEATUNIT_H
#define PREHEATUNIT_H


class PreHeatUnit
{
public:
    PreHeatUnit(int wireingPiPin);

    void setActive(bool active);

    bool isActive() const;
private:
    bool isActive_ = false;
    int wireingPiPin_ = 0;
};

#endif // PREHEATUNIT_H
