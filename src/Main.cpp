#include <iostream>
#include "Maptool.h"
#include "Mapinterface.h"

int main(int argc, char *argv[])
{

    Maptool maptool;
    Mapinterface mapinterface;

    maptool.Initialize(mapinterface);
    maptool.Run(mapinterface);
    maptool.Destroy();

    return 0;
}