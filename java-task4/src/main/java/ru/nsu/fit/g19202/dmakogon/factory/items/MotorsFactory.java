package ru.nsu.fit.g19202.dmakogon.factory.items;

public class MotorsFactory implements ItemsFactory<Motor>
{
    @Override
    public Motor getNewItem()
    {
        return new Motor();
    }
}
