package ru.nsu.fit.g19202.dmakogon.task3.gui;

import ru.nsu.fit.g19202.dmakogon.task3.View;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameSettings;
import ru.nsu.fit.g19202.dmakogon.task3.game.records.Record;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.List;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class GameFrame implements View
{
    private final JFrame frame;
    private final GameView gameView;
    private final GameController gameController;
    private Runnable exitAction;

    public GameFrame(GameController gameController) throws ResourceLoadingException
    {
        this.frame = new JFrame("Minesweeper");
        this.gameController = gameController;
        this.gameView = new GameView(gameController);
        frame.getContentPane().add(gameView);

        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("Game");

        menuBar.add(menu);
        JMenuItem menuItem;
        JRadioButtonMenuItem rbMenuItem;
        JCheckBoxMenuItem cbMenuItem;

        menuItem = new JMenuItem("New Game");
        menuItem.addActionListener(e -> onNewGameClicked());
        menu.add(menuItem);

        menu.addSeparator();
        ButtonGroup group = new ButtonGroup();

        for (GameSettings settings : gameController.getAvailableSettings())
        {
            rbMenuItem = new JRadioButtonMenuItem(settings.toString());
            if (gameController.getGameSettings().equals(settings))
            {
                rbMenuItem.setSelected(true);
            }
            rbMenuItem.addActionListener(e -> settingsChanged(settings));
            group.add(rbMenuItem);
            menu.add(rbMenuItem);
        }

        frame.addWindowListener(new WindowAdapter()
        {
            @Override
            public void windowClosing(WindowEvent e)
            {
                if (exitAction != null)
                {
                    exitAction.run();
                }
            }
        });

        menu.addSeparator();
        menuItem = new JMenuItem("High Scores");
        menuItem.addActionListener(e -> showHighscores());
        menu.add(menuItem);

        menuItem = new JMenuItem("About");
        menuItem.addActionListener(e -> showAboutDialog());
        menu.add(menuItem);

        menuItem = new JMenuItem("Exit");
        menuItem.addActionListener(e -> onExitButtonClick());

        menu.add(menuItem);

        frame.setJMenuBar(menuBar);
    }

    private void onNewGameClicked()
    {
        gameController.restartGame();
    }

    private void showHighscores()
    {
        List<Record> records = gameController.getStatistics();
        StringBuilder s = new StringBuilder();
        int highscoresCount = Math.min(records.size(), 10);
        for (int i = 0; i < highscoresCount; i++)
        {
            var record = records.get(i);
            s.append(record.getPlace()).append(". ").append(record.getName()).append(" - ").append(record.getSeconds()).append(" seconds\n");
        }
        JOptionPane.showMessageDialog(frame, s.toString(), "Highscores", JOptionPane.INFORMATION_MESSAGE);
    }

    private void showAboutDialog()
    {
        JOptionPane.showMessageDialog(frame, "Minesweeper", "About", JOptionPane.INFORMATION_MESSAGE);
    }

    @Override
    public void setExitAction(Runnable action)
    {
        exitAction = action;
    }

    private void onExitButtonClick()
    {
        frame.dispose();
        if (exitAction != null)
        {
            exitAction.run();
        }
    }

    private void settingsChanged(GameSettings newSettings)
    {
        gameController.setGameSettings(newSettings);
        gameController.restartGame();
    }

    private void runImpl()
    {
        gameController.getGame().subscribe(gameView);

        frame.setResizable(false);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
        frame.setTitle("Minesweeper");
        frame.pack();

        gameView.setOnResizeAction(frame::pack);
        frame.setVisible(true);
    }

    @Override
    public void run()
    {
        SwingUtilities.invokeLater(this::runImpl);
    }
}
