#include "gameView.h"
#include "../utility/random.h"

std::vector<bs::ShipDirection> GetDirs()
{
    static std::vector<bs::ShipDirection> availableDirs;
    if (availableDirs.empty())
    {
        availableDirs.reserve(bs::ShipDirsNames.size());
        for (auto&[dir, name] : bs::ShipDirsNames)
        {
            availableDirs.push_back(dir);
        }
    }
    return availableDirs;
}

void bs::GameView::PlaceShip(PlayerController& pc, const Board& board)
{
    bs::BoardShip bs = pc.GetShipPlaceInfo(board.GetAvailableShipsAmount(), GetDirs(), board.xSize - 1,
                                           board.ySize - 1);

    pc.ReceiveShipPlaceResult(logic->PlaceShip(bs));
}

void bs::GameView::PlaceShipsAutomatically(const bs::Board& board)
{
    auto st = logic->GetState();
    if (st != GameState::P1_PlaceShip && st != GameState::P2_PlaceShip) return;
    while (logic->GetState() == st)
    {
        logic->PlaceShip(
                rand.GetRandomBoardShip(board.GetAvailableShipsAmount(), GetDirs(), board.xSize - 1, board.ySize - 1));
    }
}

void bs::GameView::Shoot(PlayerController& pc, const Board& board, PlayerController& other)
{
    auto c = pc.GetShootPosition(board);
    auto res = logic->Shoot(c);
    pc.ReceiveAllyShotResult(res);
    other.ReceiveEnemyShotResult(c, res);
}
