#pragma once

#include <random>
#include <coordinate.h>
#include <shipTypes.h>
#include <boardShip.h>

class Random
{
private:
    std::mt19937 gen;

public:
    template<typename IntT = int>
    IntT GetRandomInRange(IntT min, IntT max)
    {
        std::uniform_int_distribution<IntT> distrib(min, max);

        return distrib(gen);
    }

    template<typename T, typename TArray>
    T GetRandomInArray(const TArray& array)
    {
        return array.at(GetRandomInRange<size_t>(0, array.size() - 1));
    }

    bs::Coordinate GetRandomCoordinate(int maxX, int maxY);

    bs::BoardShip GetRandomBoardShip(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                     const std::vector<bs::ShipDirection>& availableDirs,
                                     int maxXcoord,
                                     int maxYcoord);

    Random() : gen((std::random_device())()) {}
};

