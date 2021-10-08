#ifndef PREHEATUNIT_H
#define PREHEATUNIT_H


class PreHeatUnit
{
public:
    PreHeatUnit();

    void setActive(bool active);

    bool isActive() const;
private:
    bool isActive_ = false;
};

#endif // PREHEATUNIT_H
