#pragma once

#include "board/board.h"
#include "../playerControllers/playerController.h"

namespace bs
{
    enum class GameState
    {
        P1_PlaceShip,
        P2_PlaceShip,
        P1_Shoot,
        P2_Shoot,
        P1_Win,
        P2_Win
    };

    // controller
    class GameLogic
    {
    private:
        // todo: smart ptrs
        Board board1;
        Board board2;

        GameState state;
    public:
        GameLogic() : state(GameState::P1_PlaceShip) {}

        bs::ShipPlacementResult PlaceShip(const bs::BoardShip& shipInfo);

        bs::ShotResult Shoot(const bs::Coordinate& coord);

        const bs::Board& GetBoard() const;

        // const bs::Board& GetAllyBoard() const;

        // const bs::Board& GetEnemyBoard() const;

        GameState GetState() const;
    };
}