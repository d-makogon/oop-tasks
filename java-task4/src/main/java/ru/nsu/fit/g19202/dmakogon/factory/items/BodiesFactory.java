package ru.nsu.fit.g19202.dmakogon.factory.items;

public class BodiesFactory implements ItemsFactory<Body>
{
    @Override
    public Body getNewItem()
    {
        return new Body();
    }
}
