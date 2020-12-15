#pragma once

#include <map>
#include <string>

namespace bs
{
    enum class ShipType
    {
        T1, T2, T3, T4
    };

    const std::map<ShipType, int> ShipSizes = {
            {ShipType::T1, 1},
            {ShipType::T2, 2},
            {ShipType::T3, 3},
            {ShipType::T4, 4},
    };

    const std::map<ShipType, int> ShipsMaxAmount = {
            {ShipType::T1, 4},
            {ShipType::T2, 3},
            {ShipType::T3, 2},
            {ShipType::T4, 1},
    };

    const std::map<ShipType, const char* const> ShipNames = {
            {ShipType::T1, "Type 1"},
            {ShipType::T2, "Type 2"},
            {ShipType::T3, "Type 3"},
            {ShipType::T4, "Type 4"},
    };
}