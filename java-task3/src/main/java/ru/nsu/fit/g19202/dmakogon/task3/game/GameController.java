package ru.nsu.fit.g19202.dmakogon.task3.game;

import ru.nsu.fit.g19202.dmakogon.task3.game.records.Record;

import java.util.List;

public class GameController
{
    private final Game game;
    private final List<GameSettings> settings;

    public GameController(Game game, List<GameSettings> settings)
    {
        this.game = game;
        this.settings = settings;
    }

    public Game getGame()
    {
        return game;
    }

    public void restartGame()
    {
        game.restart();
    }

    public Game.GameState getGameState()
    {
        return game.getGameState();
    }

    public FieldCell getCell(int x, int y)
    {
        return game.getCell(x, y);
    }

    public void openCell(int x, int y)
    {
        game.openCell(x, y);
    }

    public void markCell(int x, int y)
    {
        game.markCell(x, y);
    }

    public GameSettings getGameSettings()
    {
        return game.getSettings();
    }

    public void setGameSettings(GameSettings newSettings)
    {
        game.setSettings(newSettings);
    }

    public List<GameSettings> getAvailableSettings()
    {
        return settings;
    }

    public void storeResult(String name)
    {
        game.storeRecord(name);
    }

    public List<Record> getStatistics()
    {
        return game.getStatistics();
    }
}
