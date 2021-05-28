package ru.nsu.fit.g19202.dmakogon.factory.items;

import ru.nsu.fit.g19202.dmakogon.factory.IdGenerator;

public class Car implements Item
{
    private final String id = IdGenerator.nextId();

    private final Body body;
    private final Accessory accessory;
    private final Motor motor;

    public Car(Body body, Accessory accessory, Motor motor)
    {
        this.body = body;
        this.accessory = accessory;
        this.motor = motor;
    }

    @Override
    public String getId()
    {
        return id;
    }

    @Override
    public String toString()
    {
        return "Car{" + "id='" + id + '\'' + ", body=" + body + ", accessory=" + accessory + ", motor=" + motor + '}';
    }
}
