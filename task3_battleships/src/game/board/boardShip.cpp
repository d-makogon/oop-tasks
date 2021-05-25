#include "include/boardShip.h"

bs::Coordinate bs::BoardShip::GetCoordinate() const
{
    return coordinate;
}

bs::ShipDirection bs::BoardShip::GetDirection() const
{
    return direction;
}

bs::ShipType bs::BoardShip::GetType() const
{
    return type;
}
