package ru.nsu.fit.g19202.dmakogon.factory;

import ru.nsu.fit.g19202.dmakogon.factory.items.Accessory;
import ru.nsu.fit.g19202.dmakogon.factory.items.Body;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;
import ru.nsu.fit.g19202.dmakogon.factory.items.Motor;

public interface CarFactoryObserver
{
    void bodyAdded(CarFactory carFactory, Body body);

    void bodyRemoved(CarFactory carFactory, Body body);

    void motorAdded(CarFactory carFactory, Motor motor);

    void motorRemoved(CarFactory carFactory, Motor motor);

    void accessoryAdded(CarFactory carFactory, Accessory accessory);

    void accessoryRemoved(CarFactory carFactory, Accessory accessory);

    void carAdded(CarFactory carFactory, Car car);

    void carRemoved(CarFactory carFactory, Car car);
}
