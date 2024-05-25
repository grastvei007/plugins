#include "pigpio.h"
#include "wiringpiwrapper.h"
#include "pin.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>


namespace plugin{


bool PiGpio::initialize()
{
    return true;
}


void PiGpio::mainloop()
{

}

}// end namespace
