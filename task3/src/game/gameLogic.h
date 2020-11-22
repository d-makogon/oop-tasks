#pragma once

#include <memory>

#include "board/board.h"

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
        std::unique_ptr<Board> board1;
        std::unique_ptr<Board> board2;

        GameState state;

    public:
        GameLogic() : state(GameState::P1_PlaceShip),
                      board1(std::make_unique<Board>()),
                      board2(std::make_unique<Board>()) {}

        bs::ShipPlacementResult PlaceShip(const bs::BoardShip& shipInfo);

        bs::ShotResult Shoot(const bs::Coordinate& coord);

        const Board& GetAllyBoard() const;
        const Board& GetEnemyBoard() const;

        GameState GetState() const;
    };
}