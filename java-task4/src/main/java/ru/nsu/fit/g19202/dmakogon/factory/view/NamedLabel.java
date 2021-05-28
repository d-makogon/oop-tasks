package ru.nsu.fit.g19202.dmakogon.factory.view;

import javax.swing.*;

public class NamedLabel extends JLabel
{
    private final String name;

    public NamedLabel(String name)
    {
        super("");
        this.name = name;
    }

    public NamedLabel(String name, String text)
    {
        super(text);
        this.name = name;
    }

    @Override
    public String getText()
    {
        return name + ": " + super.getText();
    }
}
