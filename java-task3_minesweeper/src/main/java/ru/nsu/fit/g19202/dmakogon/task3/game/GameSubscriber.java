package ru.nsu.fit.g19202.dmakogon.task3.game;

public interface GameSubscriber
{
    void onFieldUpdate(Game game);

    void onStateChange(Game game, Game.GameState newState);

    void onTimeUpdate(Game game, int newSeconds);

    void onSettingsChange(Game game, GameSettings settings);
}
