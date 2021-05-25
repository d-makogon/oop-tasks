#include "include/shipCreator.h"

bs::Ship bs::ShipCreator::CreateShip(const ShipType& type)
{
    return Ship(type, ShipSizes.at(type));
}
