#include "gameLogic.h"

#include <exception>

bs::GameState bs::GameLogic::GetState() const
{
    return state;
}

const bs::Board& bs::GameLogic::GetAllyBoard() const
{
    if (state == GameState::P1_PlaceShip || state == GameState::P1_Shoot)
    {
        return *board1;
    }
    else if (state == GameState::P2_Shoot || state == GameState::P2_PlaceShip)
    {
        return *board2;
    }
    // todo: think how to do it better (return bool?)
    throw std::runtime_error("Wrong game state");
}

const bs::Board& bs::GameLogic::GetEnemyBoard() const
{
    if (state == GameState::P1_PlaceShip || state == GameState::P1_Shoot)
    {
        return *board2;
    }
    else if (state == GameState::P2_Shoot || state == GameState::P2_PlaceShip)
    {
        return *board1;
    }
    throw std::runtime_error("Wrong game state");
}

bs::ShotResult bs::GameLogic::Shoot(const bs::Coordinate& coord)
{
    bs::ShotResult shotResult;

    if (state == GameState::P1_Shoot)
    {
        shotResult = board2->Fire(coord);
    }
    else if (state == GameState::P2_Shoot)
    {
        shotResult = board1->Fire(coord);
    }
    else
    {
        throw std::runtime_error("Wrong game state");
    }

    switch (shotResult)
    {
        case bs::ShotResult::Victory:
            state = (state == GameState::P1_Shoot) ? GameState::P1_Win : GameState::P2_Win;
            break;
        case bs::ShotResult::Miss:
            state = (state == GameState::P1_Shoot) ? GameState::P2_Shoot : GameState::P1_Shoot;
        default:
            break;
    }
    return shotResult;
}

bs::ShipPlacementResult bs::GameLogic::PlaceShip(const bs::BoardShip& shipInfo)
{
    bs::ShipPlacementResult result;
    if (state == GameState::P1_PlaceShip)
    {
        result = board1->PlaceShip(shipInfo);
        state = (board1->CanPlaceShips()) ? GameState::P1_PlaceShip : GameState::P2_PlaceShip;
    }
    else if (state == GameState::P2_PlaceShip)
    {
        result = board2->PlaceShip(shipInfo);
        state = (board2->CanPlaceShips()) ? GameState::P2_PlaceShip : GameState::P1_Shoot;
    }
    else
    {
        throw std::runtime_error("Wrong game state");
    }
    return result;
}
