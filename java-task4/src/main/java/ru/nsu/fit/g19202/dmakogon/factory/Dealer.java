package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;

import java.util.concurrent.atomic.AtomicInteger;

public class Dealer extends Thread
{
    private final Logger logger = LogManager.getLogger();

    private final Storage<Car> carStorage;
    private final AtomicInteger itemQueryTime;

    public Dealer(String name, Storage<Car> carStorage, int itemQueryTime)
    {
        super(name);
        this.carStorage = carStorage;
        this.itemQueryTime = new AtomicInteger(itemQueryTime);
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                //noinspection BusyWait
                sleep(itemQueryTime.get());
                Car car = carStorage.getItem();
                logger.info(getName() + ": " + car);
            }
            catch (InterruptedException e)
            {
                logger.info(getName() +  " interrupted");
                break;
            }
        }
    }

    public void setItemQueryTime(int newValue)
    {
        itemQueryTime.set(newValue);
    }
}
