package ru.nsu.fit.g19202.dmakogon.task3.gui;

import ru.nsu.fit.g19202.dmakogon.task3.game.Game;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameSettings;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameSubscriber;

import javax.swing.*;
import java.awt.*;

public class GameView extends JPanel implements GameSubscriber
{
    private final GameController gameController;
    private GridBagConstraints gbc;
    private TopPanelView topPanelView;
    private BoardView boardView;
    private Runnable onResizeAction;

    public GameView(GameController gameController) throws ResourceLoadingException
    {
        this.gameController = gameController;

        setLayout(new GridBagLayout());

        this.gbc = new GridBagConstraints();

        topPanelView = new TopPanelView(gameController);
        boardView = new BoardView(gameController);

        topPanelView.setSizeX(boardView.getPreferredSize().width);

        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.gridx = 0;
        gbc.gridy = 0;
        add(topPanelView, gbc);

        gbc.gridy = 1;
        add(boardView, gbc);
    }

    public void setOnResizeAction(Runnable action)
    {
        this.onResizeAction = action;
    }

    @Override
    public void onFieldUpdate(Game game)
    {
        boardView.drawField();
        topPanelView.onFieldUpdate();
    }

    @Override
    public void onStateChange(Game game, Game.GameState newState)
    {
        topPanelView.onStateChange(newState);
        if (newState == Game.GameState.LOST)
        {
            JOptionPane.showMessageDialog(this, "You lost!");
        }
        else if (newState == Game.GameState.WON)
        {
            String name = JOptionPane.showInputDialog(this, "You won! Your time is " + game.getSeconds() + " seconds.\nEnter your name:");
            gameController.storeResult(name);
        }
    }

    @Override
    public void onTimeUpdate(Game game, int newSeconds)
    {
        topPanelView.onTimeUpdate(newSeconds);
    }

    @Override
    public void onSettingsChange(Game game, GameSettings settings)
    {
        boardView.drawField();
        topPanelView.setSizeX(boardView.getPreferredSize().width);
        onResizeAction.run();
    }
}
