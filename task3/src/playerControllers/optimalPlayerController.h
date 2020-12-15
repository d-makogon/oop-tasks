#pragma once

#include "playerController.h"
#include "../utility/random.h"

namespace bs
{
    class OptimalPlayerController : public PlayerController
    {
    private:
        Random random;

    public:
        OptimalPlayerController() : random() {}

        BoardShip GetShipPlaceInfo(const std::vector<std::pair<bs::ShipType, int>>& availableTypes,
                                   const std::vector<ShipDirection>& availableDirs,
                                   int maxXcoord,
                                   int maxYcoord) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override {}

        Coordinate GetShootPosition(const bs::Board& enemyBoard) override;

        void ReceiveAllyShotResult(const ShotResult& shotResult) override {}

        void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) override {}

        bool GetTrueOrFalse(const std::string& msg) override { return true; }

        bool IsHuman() const override { return false; }

        ~OptimalPlayerController() override = default;
    };
}