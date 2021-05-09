package ru.nsu.fit.g19202.dmakogon.task3.gui;

import ru.nsu.fit.g19202.dmakogon.task3.game.Game;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class TopPanelView extends JComponent
{
    private final Image[] digitsImages;
    private final Image loseFaceImage;
    private final Image pressedFaceImage;
    private final Image unpressedFaceImage;
    private final int digitSizeX;
    private final int digitSizeY;
    private final GameController gameController;
    private Image currentFaceImage;
    private int ownSizeX;
    private int ownSizeY;
    private int faceX;
    private int faceY;
    private int seconds;
    private int remainingMines;

    public TopPanelView(GameController gameController) throws ResourceLoadingException
    {
        this.gameController = gameController;

        digitsImages = new Image[10];
        for (int i = 0; i < digitsImages.length; i++)
        {
            digitsImages[i] = ResourceLoader.loadImage("/sprites/d" + i + ".png", this);
        }

        loseFaceImage = ResourceLoader.loadImage("/sprites/face_lose.png", this);
        pressedFaceImage = ResourceLoader.loadImage("/sprites/face_pressed.png", this);
        unpressedFaceImage = ResourceLoader.loadImage("/sprites/face_unpressed.png", this);
        currentFaceImage = unpressedFaceImage;

        digitSizeX = digitsImages[0].getWidth(null);
        digitSizeY = digitsImages[0].getHeight(null);

        ownSizeX = 6 * digitSizeX + currentFaceImage.getWidth(null) + 20;
        ownSizeY = digitSizeY + 10;

        this.seconds = 0;

        faceX = (ownSizeX - currentFaceImage.getWidth(null)) / 2;
        faceY = (ownSizeY - currentFaceImage.getHeight(null)) / 2;

        addMouseListener(new MouseAdapter()
        {
            @Override
            public void mouseReleased(MouseEvent e)
            {
                TopPanelView.this.mouseReleased(e);
            }

            @Override
            public void mousePressed(MouseEvent e)
            {
                TopPanelView.this.mousePressed(e);
            }
        });
    }

    private void mousePressed(MouseEvent e)
    {
        int mouseX = e.getX();
        int mouseY = e.getY();
        if ((mouseX >= faceX && mouseX <= faceX + currentFaceImage.getWidth(null)) && (mouseY >= faceY && mouseY <= faceY + currentFaceImage.getHeight(null)))
        {
            currentFaceImage = pressedFaceImage;
        }
        else
        {
            currentFaceImage = unpressedFaceImage;
        }
        repaint();
    }

    private void mouseReleased(MouseEvent e)
    {
        int mouseX = e.getX();
        int mouseY = e.getY();
        if ((mouseX >= faceX && mouseX <= faceX + currentFaceImage.getWidth(null)) && (mouseY >= faceY && mouseY <= faceY + currentFaceImage.getHeight(null)))
        {
            gameController.restartGame();
        }
        currentFaceImage = unpressedFaceImage;
        repaint();
    }

    @Override
    public void paintComponent(Graphics g)
    {
        final int timerStartX = 5;
        final int centerY = (ownSizeY - digitSizeY) / 2;
        final int minesStartX = (ownSizeX - 5) - digitSizeX * 3;

        // draw time
        drawDigits(g, timerStartX, centerY, seconds);

        // draw central button
        faceX = (ownSizeX - currentFaceImage.getWidth(null)) / 2;
        faceY = (ownSizeY - currentFaceImage.getHeight(null)) / 2;
        g.drawImage(currentFaceImage, faceX, faceY, null);

        // draw mines count
        drawDigits(g, minesStartX, centerY, remainingMines);
    }

    private void drawDigits(Graphics g, int startX, int y, int number)
    {
        int hundreds = (number / 100) % 100;
        int tens = (number / 10) % 10;
        int ones = number % 10;
        g.drawImage(digitsImages[hundreds], startX, y, null);
        g.drawImage(digitsImages[tens], digitSizeX + startX, y, null);
        g.drawImage(digitsImages[ones], digitSizeX * 2 + startX, y, null);
    }

    @Override
    public Dimension getPreferredSize()
    {
        return new Dimension(ownSizeX, ownSizeY);
    }

    public void setSizeX(int newSizeX)
    {
        this.ownSizeX = newSizeX;
    }

    public void setSizeY(int newSizeY)
    {
        this.ownSizeY = newSizeY;
    }

    public void onTimeUpdate(int seconds)
    {
        this.seconds = seconds;
        repaint();
    }

    public void onFieldUpdate()
    {
        this.remainingMines = gameController.getGame().getRemainingMinesCount();
        repaint();
    }

    public void onStateChange(Game.GameState newState)
    {
        if (newState == Game.GameState.LOST)
        {
            currentFaceImage = loseFaceImage;
        }
        else
        {
            currentFaceImage = unpressedFaceImage;
        }
        repaint();
    }
}
