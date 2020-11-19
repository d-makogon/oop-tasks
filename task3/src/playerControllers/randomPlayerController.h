#pragma once

#include "playerController.h"

#include <random>

namespace bs
{
    class RandomPlayerController : public PlayerController
    {
    private:
        std::mt19937 gen;

        int GetRandomInRange(int min, int max);

        template<typename T, typename TArray>
        T GetRandomInArray(const TArray& array)
        {
            return array[GetRandomInRange(0, array.size() - 1)];
        }

    public:
        RandomPlayerController() : gen((std::random_device())()) {}

        BoardShip GetShipPlaceInfo(const std::vector<ShipType>& availableTypes,
                                   const std::vector<ShipDirection>& availableDirs,
                                   int maxXcoord,
                                   int maxYcoord) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override;
    };
}
