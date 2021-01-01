#include "include/gameView.h"
#include <random.h>

std::vector<bs::ShipDirection> GetDirsVector()
{
    static std::vector<bs::ShipDirection> dirs;
    if (dirs.empty())
    {
        dirs.reserve(bs::ShipDirsNames.size());
        for (auto&[dir, name] : bs::ShipDirsNames)
        {
            dirs.push_back(dir);
        }
    }
    return dirs;
}

void bs::GameView::PlaceShip(PlayerController& pc, const Board& board)
{
    bs::BoardShip bs = pc.GetShipPlaceInfo(board);

    pc.ReceiveShipPlaceResult(logic->PlaceShip(bs));
}

void bs::GameView::PlaceShipsAutomatically(const bs::Board& board)
{
    auto st = logic->GetState();
    if (st != GameState::P1_PlaceShip && st != GameState::P2_PlaceShip) return;
    while (logic->GetState() == st)
    {
        logic->PlaceShip(
                rand.GetRandomBoardShip(board.GetAvailableShipsAmount(), GetDirsVector(), board.xSize - 1,
                                        board.ySize - 1));
    }
}

void bs::GameView::Shoot(PlayerController& pc, const Board& board, PlayerController& other)
{
    auto c = pc.GetShootPosition(board);
    auto res = logic->Shoot(c);
    pc.ReceiveAllyShotResult(res);
    other.ReceiveEnemyShotResult(c, res);
}
