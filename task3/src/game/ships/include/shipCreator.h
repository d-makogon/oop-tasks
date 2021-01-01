#pragma once

#include <ship.h>
#include <shipTypes.h>

namespace bs
{
    class ShipCreator
    {
    public:
        static Ship CreateShip(const ShipType& type);
    };
}