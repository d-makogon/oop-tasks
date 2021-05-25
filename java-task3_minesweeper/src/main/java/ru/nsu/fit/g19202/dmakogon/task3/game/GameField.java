package ru.nsu.fit.g19202.dmakogon.task3.game;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Random;

public class GameField
{
    private final int sizeX;
    private final int sizeY;
    private final int minesCount;
    private final FieldCell[][] cells;
    private int closedEmptyCellsCount;
    private int markedCellsCount;
    private boolean isGenerated;

    public GameField(int sizeX, int sizeY, int minesCount)
    {
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        this.isGenerated = false;
        this.minesCount = minesCount;
        closedEmptyCellsCount = sizeX * sizeY - minesCount;
        cells = new FieldCell[sizeX][sizeY];
        for (int i = 0; i < sizeX; i++)
        {
            for (int j = 0; j < sizeY; j++)
            {
                cells[i][j] = new FieldCell(FieldCell.CellType.EMPTY, i, j);
            }
        }
        this.markedCellsCount = 0;
    }

    private int countSurMines(int x, int y)
    {
        int minesCount = 0;
        for (int xOffset = -1; xOffset < 2; xOffset++)
        {
            for (int yOffset = -1; yOffset < 2; yOffset++)
            {
                if (isMine(x + xOffset, y + yOffset))
                {
                    minesCount++;
                }
            }
        }
        return minesCount;
    }

    private boolean isMine(int x, int y)
    {
        return (isValidCoord(x, y)) && (cells[x][y].getType() == FieldCell.CellType.MINE);
    }

    private boolean isValidCoord(int x, int y)
    {
        return (x >= 0 && y >= 0 && x < sizeX && y < sizeY);
    }

    public void generate(int startX, int startY)
    {
        if (!isValidCoord(startX, startY))
        {
            return;
        }
        int minesCount = 0;
        Random random = new Random();
        while (minesCount < this.minesCount)
        {
            int mineX = random.nextInt(sizeX);
            int mineY = random.nextInt(sizeY);
            if ((startX != mineX) && (startY != mineY) && !isMine(mineX, mineY))
            {
                cells[mineX][mineY].setType(FieldCell.CellType.MINE);
                minesCount++;
            }
        }
        for (int i = 0; i < sizeX; i++)
        {
            for (int j = 0; j < sizeY; j++)
            {
                cells[i][j].setSurroundingMinesCount(countSurMines(i, j));
            }
        }
        isGenerated = true;
    }

    public boolean isGenerated()
    {
        return isGenerated;
    }

    public FieldCell getCellAt(int x, int y)
    {
        if (!isValidCoord(x, y))
        {
            return null;
        }
        return cells[x][y];
    }

    public int getRemainingMinesCount()
    {
        return minesCount - markedCellsCount;
    }

    public FieldCell.CellType openCell(int x, int y)
    {
        if (!isValidCoord(x, y))
        {
            return null;
        }

        FieldCell cell = cells[x][y];
        boolean cellClosed = cell.isClosed();
        if (cellClosed && !cell.isMarked())
        {
            FieldCell.CellType type = cell.getType();
            if (type == FieldCell.CellType.MINE)
            {
                cell.setOpen(true);
                cell.setExploded(true);
                cell.setVisible(true);
            }
            else if (type == FieldCell.CellType.EMPTY)
            {
                openCellDFS(x, y);
            }
            return type;
        }
        else if (!cellClosed)
        {
            Collection<FieldCell> surCells = getSurroundingCells(x, y);
            int surClosedMarkedCount = (int) surCells.stream().filter(c -> c.isClosed() && c.isMarked()).count();
            int surClosedCount = (int) surCells.stream().filter(FieldCell::isClosed).count();

            int surroundingMinesCount = cell.getSurroundingMinesCount();
            if (surClosedMarkedCount == surroundingMinesCount)
            {
                // open all not marked cells

                for (FieldCell surCell : surCells)
                {
                    if (!surCell.isMarked())
                    {
                        if (surCell.getType() == FieldCell.CellType.MINE)
                        {
                            return FieldCell.CellType.MINE;
                        }
                        if (surCell.isClosed())
                        {
                            openCellDFS(surCell.getX(), surCell.getY());
                        }
                    }
                }

            }
            else if (surClosedCount == surroundingMinesCount)
            {
                // mark all closed cells
                surCells.forEach(c -> markCell(c, false));
            }
        }
        return FieldCell.CellType.EMPTY;
    }

    private Collection<FieldCell> getSurroundingCells(int x, int y)
    {
        ArrayList<FieldCell> cellsList = new ArrayList<>(8);
        for (int xOffset = -1; xOffset < 2; xOffset++)
        {
            for (int yOffset = -1; yOffset < 2; yOffset++)
            {
                // not including (x, y) and invalid coords
                if (isValidCoord(x + xOffset, y + yOffset) && (xOffset != 0 || yOffset != 0))
                {
                    cellsList.add(cells[x + xOffset][y + yOffset]);
                }
            }
        }
        return cellsList;
    }

    public int getClosedEmptyCellsCount()
    {
        return closedEmptyCellsCount;
    }

    private void openCellDFS(int x, int y)
    {
        if (!isValidCoord(x, y))
        {
            return;
        }
        // if closed
        if (cells[x][y].isClosed() && cells[x][y].getType() == FieldCell.CellType.EMPTY)
        {
            cells[x][y].setOpen(true);
            cells[x][y].setVisible(true);
            closedEmptyCellsCount--;
            if (cells[x][y].getSurroundingMinesCount() != 0)
            {
                return;
            }
            for (int xOffset = -1; xOffset < 2; xOffset++)
            {
                for (int yOffset = -1; yOffset < 2; yOffset++)
                {
                    openCellDFS(x + xOffset, y + yOffset);
                }
            }
        }
    }

    public void markCell(int x, int y)
    {
        markCell(cells[x][y], true);
    }

    private void markCell(FieldCell cell, boolean invert)
    {
        if (cell.isClosed())
        {
            if (invert)
            {
                if (cell.isMarked())
                {
                    markedCellsCount--;
                }
                else
                {
                    markedCellsCount++;
                }
                cell.changeMarked();
            }
            else
            {
                if (!cell.isMarked())
                {
                    markedCellsCount++;
                }
                cell.setMarked(true);
            }
            cell.setVisible(cell.isMarked());
        }
    }

    public int getSizeX()
    {
        return sizeX;
    }

    public int getSizeY()
    {
        return sizeY;
    }

    public void openClosedMines()
    {
        for (int x = 0; x < sizeX; x++)
        {
            for (int y = 0; y < sizeY; y++)
            {
                FieldCell cell = cells[x][y];
                if (cell.getType() == FieldCell.CellType.MINE)
                {
                    cell.setVisible(true);
                }
                else if (cell.getType() == FieldCell.CellType.EMPTY && cell.isMarked())
                {
                    cell.setWrongMarked(true);
                }
            }
        }
    }
}
