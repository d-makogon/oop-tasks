package ru.nsu.fit.g19202.dmakogon.factory.items;

public class AccessoriesFactory implements ItemsFactory<Accessory>
{
    @Override
    public Accessory getNewItem()
    {
        return new Accessory();
    }
}
