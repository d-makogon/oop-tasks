package ru.nsu.fit.g19202.dmakogon.factory.items;

public interface ItemsFactory<T extends Item>
{
    T getNewItem();
}
