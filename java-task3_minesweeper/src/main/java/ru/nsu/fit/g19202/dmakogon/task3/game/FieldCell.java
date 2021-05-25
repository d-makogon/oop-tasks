package ru.nsu.fit.g19202.dmakogon.task3.game;

public class FieldCell
{
    private CellType type;
    private boolean isOpen;
    private boolean isMarked;
    private boolean isVisible;
    private boolean isExploded;
    private int surroundingMinesCount;
    private boolean isWrongMarked;
    private int x;
    private int y;

    public FieldCell(CellType type, int x, int y)
    {
        this.type = type;
        this.isOpen = false;
        this.isMarked = false;
        this.isVisible = false;
        this.isExploded = false;
        this.x = x;
        this.y = y;
    }

    public int getX()
    {
        return x;
    }

    public void setX(int x)
    {
        this.x = x;
    }

    public void setCoords(int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    public int getY()
    {
        return y;
    }

    public void setY(int y)
    {
        this.y = y;
    }

    public int getSurroundingMinesCount()
    {
        return surroundingMinesCount;
    }

    public void setSurroundingMinesCount(int surroundingMinesCount)
    {
        this.surroundingMinesCount = surroundingMinesCount;
    }

    public boolean isExploded()
    {
        return isExploded;
    }

    public void setExploded(boolean exploded)
    {
        isExploded = exploded;
    }

    public boolean isVisible()
    {
        return isVisible;
    }

    public void setVisible(boolean visible)
    {
        isVisible = visible;
    }

    public CellType getType()
    {
        return type;
    }

    public void setType(CellType type)
    {
        this.type = type;
    }

    public boolean isMarked()
    {
        return isMarked;
    }

    public void setMarked(boolean marked)
    {
        isMarked = marked;
    }

    public boolean isWrongMarked()
    {
        return isWrongMarked;
    }

    public void setWrongMarked(boolean wrongMarked)
    {
        isWrongMarked = wrongMarked;
    }

    public boolean isClosed()
    {
        return !isOpen;
    }

    public void setOpen(boolean open)
    {
        isOpen = open;
    }

    public void changeMarked()
    {
        isMarked = !isMarked;
    }

    public enum CellType
    {MINE, EMPTY}
}
