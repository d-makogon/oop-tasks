#include "include/gameLogic.h"

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
            playedRounds++;
            if (state == GameState::P1_Shoot)
                p1Score++;
            else
                p2Score++;
            if (playedRounds >= maxRounds)
            {
                state = (p1Score > p2Score) ? GameState::P1_WinGame : GameState::P2_WinGame;
            }
            else
            {
                state = (state == GameState::P1_Shoot) ? GameState::P1_WinRound : GameState::P2_WinRound;
                ResetBoards();
            }
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

int bs::GameLogic::GetP1Score() const
{
    return p1Score;
}

int bs::GameLogic::GetP2Score() const
{
    return p2Score;
}

int bs::GameLogic::GetPlayedGamesCount() const
{
    return playedRounds;
}

int bs::GameLogic::GetGamesCount() const
{
    return maxRounds;
}

void bs::GameLogic::StartRound()
{
    ResetBoards();
    if (playedRounds < maxRounds)
        state = GameState::P1_PlaceShip;
    else if (playedRounds == maxRounds)
        state = GameState::P1_WinGame;
}

void bs::GameLogic::ResetBoards()
{
    board1.reset();
    board2.reset();
    board1 = std::make_unique<Board>();
    board2 = std::make_unique<Board>();
}
