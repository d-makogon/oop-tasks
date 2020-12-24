#pragma once

#include <memory>

#include <board.h>

namespace bs
{
    enum class GameState
    {
        RoundStart,
        P1_PlaceShip,
        P2_PlaceShip,
        P1_Shoot,
        P2_Shoot,
        P1_WinRound,
        P2_WinRound,
        P1_WinGame,
        P2_WinGame,
    };

    // controller
    class GameLogic
    {
    private:
        std::unique_ptr<Board> board1;
        std::unique_ptr<Board> board2;
        int maxRounds;
        int playedRounds = 0;
        int p1Score = 0;
        int p2Score = 0;

        GameState state;

        void ResetBoards();

    public:
        explicit GameLogic(int maxRounds) : state(GameState::RoundStart),
                                            board1(std::make_unique<Board>()),
                                            board2(std::make_unique<Board>()),
                                            maxRounds(maxRounds) {}

        void StartRound();

        bs::ShipPlacementResult PlaceShip(const bs::BoardShip& shipInfo);

        bs::ShotResult Shoot(const bs::Coordinate& coord);

        const Board& GetAllyBoard() const;

        const Board& GetEnemyBoard() const;

        int GetP1Score() const;

        int GetP2Score() const;

        int GetPlayedGamesCount() const;

        int GetGamesCount() const;

        GameState GetState() const;
    };
}