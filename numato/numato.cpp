#include "numato.h"

extern "C" PluginInterface* createPlugin()
{
    return new Numato();
}


bool Numato::initialize()
{

    return true;
}

void Numato::mainloop()
{

}
