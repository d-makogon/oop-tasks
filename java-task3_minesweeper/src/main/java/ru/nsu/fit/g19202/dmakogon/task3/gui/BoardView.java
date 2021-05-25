package ru.nsu.fit.g19202.dmakogon.task3.gui;

import ru.nsu.fit.g19202.dmakogon.task3.game.FieldCell;
import ru.nsu.fit.g19202.dmakogon.task3.game.Game;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

public class BoardView extends JComponent
{
    private static final Cursor handCursor = new Cursor(Cursor.HAND_CURSOR);
    private static final Cursor defaultCursor = Cursor.getDefaultCursor();
    private int sizeX;
    private int sizeY;
    private int cellsX;
    private int cellsY;
    private final int cellSize;
    private final Image openedMineImage;
    private final Image closedMineImage;
    private final Image wrongMineImage;
    private final Image emptyBlockImage;
    private final Image closedBlockImage;
    private final Image[] digitsImages;
    private final Image markImage;
    private final GameController gameController;
    private Cursor curCursor = handCursor;
    private int hoveredCellX = -1;
    private int hoveredCellY = -1;
    private boolean isMousePressed = false;

    public BoardView(GameController gameController) throws ResourceLoadingException
    {
        this.gameController = gameController;

        openedMineImage = ResourceLoader.loadImage("/sprites/mine_red.png", this);
        closedMineImage = ResourceLoader.loadImage("/sprites/mine.png", this);
        wrongMineImage = ResourceLoader.loadImage("/sprites/mine_wrong.png", this);
        emptyBlockImage = ResourceLoader.loadImage("/sprites/empty.png", this);
        closedBlockImage = ResourceLoader.loadImage("/sprites/closed.png", this);
        markImage = ResourceLoader.loadImage("/sprites/flag.png", this);

        digitsImages = new Image[8];

        for (int i = 0; i < digitsImages.length; i++)
        {
            digitsImages[i] = ResourceLoader.loadImage("/sprites/" + (i + 1) + ".png", this);
        }

        cellSize = openedMineImage.getHeight(null);

        cellsX = gameController.getGameSettings().getFieldSizeX();
        cellsY = gameController.getGameSettings().getFieldSizeY();
        sizeX = cellsX * cellSize;
        sizeY = cellsY * cellSize;

        addMouseMotionListener(new MouseMotionAdapter()
        {
            @Override
            public void mouseMoved(MouseEvent e)
            {
                BoardView.this.mouseMoved(e);
            }

            @Override
            public void mouseDragged(MouseEvent e)
            {
                BoardView.this.mouseMoved(e);
            }
        });

        addMouseListener(new MouseAdapter()
        {
            @Override
            public void mouseEntered(MouseEvent e)
            {
                BoardView.this.mouseEntered(e);
            }

            @Override
            public void mouseReleased(MouseEvent e)
            {
                BoardView.this.mouseReleased(e);
            }

            @Override
            public void mousePressed(MouseEvent e)
            {
                BoardView.this.mousePressed(e);
            }

            @Override
            public void mouseExited(MouseEvent e)
            {
                BoardView.this.mouseExited(e);
            }
        });
    }

    private void mouseMoved(MouseEvent e)
    {
        hoveredCellX = e.getX() / cellSize;
        hoveredCellY = e.getY() / cellSize;
        setCursor(curCursor);
        repaint();
    }

    private void mouseExited(MouseEvent e)
    {
        setCursor(Cursor.getDefaultCursor());
    }

    private void mouseEntered(MouseEvent e)
    {
        setCursor(curCursor);
    }

    private void mousePressed(MouseEvent e)
    {
        int button = e.getButton();
        if (button == MouseEvent.BUTTON1)
        {
            isMousePressed = true;
        }
    }

    private void mouseReleased(MouseEvent e)
    {
        int button = e.getButton();
        int x = e.getX() / cellSize;
        int y = e.getY() / cellSize;
        if (button == MouseEvent.BUTTON1)
        {
            isMousePressed = false;
            gameController.openCell(x, y);
        }
        else if (button == MouseEvent.BUTTON3)
        {
            gameController.markCell(x, y);
        }
    }

    private void drawCell(Graphics g, int x, int y)
    {
        FieldCell cell = gameController.getCell(x, y);
        FieldCell.CellType cellType = cell.getType();
        boolean isCellOpen = !cell.isClosed();
        boolean isCellMarked = cell.isMarked();
        boolean isCellVisible = cell.isVisible();
        boolean isCellExploded = cell.isExploded();
        boolean isWrongMarked = cell.isWrongMarked();

        Image cellImage = closedBlockImage;

        if (isCellVisible)
        {
            if (cellType == FieldCell.CellType.MINE)
            {
                if (isCellExploded)
                {
                    cellImage = openedMineImage;
                }
                else if (isCellMarked)
                {
                    cellImage = markImage;
                }
                else
                {
                    cellImage = closedMineImage;
                }
            }
            else if (cellType == FieldCell.CellType.EMPTY)
            {
                if (isCellOpen)
                {
                    int surroundingMinesCount = cell.getSurroundingMinesCount();
                    if (surroundingMinesCount > 0)
                    {
                        cellImage = digitsImages[surroundingMinesCount - 1];
                    }
                    else
                    {
                        cellImage = emptyBlockImage;
                    }
                }
                else if (isWrongMarked)
                {
                    cellImage = wrongMineImage;
                }
                else if (isCellMarked)
                {
                    cellImage = markImage;
                }
            }
        }
        int xCoord = x * cellSize;
        int yCoord = y * cellSize;
        g.drawImage(cellImage, xCoord, yCoord, null);
    }

    @Override
    public void paintComponent(Graphics g)
    {
        g.setColor(new Color(100, 100, 100, 50));
        for (int i = 0; i < cellsX; i++)
        {
            for (int j = 0; j < cellsY; j++)
            {
                drawCell(g, i, j);
            }
        }
        Game.GameState gameState = gameController.getGameState();
        if (hoveredCellX < 0 || hoveredCellY < 0)
        {
            return;
        }
        if (gameState == Game.GameState.STARTED || gameState == Game.GameState.NOT_STARTED)
        {
            if (!isMousePressed)
            {
                g.fillRect(hoveredCellX * cellSize, hoveredCellY * cellSize, cellSize, cellSize);
            }
            else
            {
                FieldCell cell = gameController.getCell(hoveredCellX, hoveredCellY);
                if (cell != null && cell.isClosed())
                {
                    g.drawImage(emptyBlockImage, hoveredCellX * cellSize, hoveredCellY * cellSize, null);
                }
            }
        }
    }

    @Override
    public Dimension getPreferredSize()
    {
        return new Dimension(sizeX, sizeY);
    }

    private void setCursor(Game.GameState gameState)
    {
        if (gameState == Game.GameState.NOT_STARTED || gameState == Game.GameState.STARTED)
        {
            curCursor = handCursor;
        }
        else
        {
            curCursor = defaultCursor;
        }
    }

    public void drawField()
    {
        setCursor(gameController.getGameState());
        cellsX = gameController.getGameSettings().getFieldSizeX();
        cellsY = gameController.getGameSettings().getFieldSizeY();
        sizeX = cellSize * cellsX;
        sizeY = cellSize * cellsY;
        repaint();
    }
}
