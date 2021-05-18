package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Accessory;
import ru.nsu.fit.g19202.dmakogon.factory.items.Body;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;
import ru.nsu.fit.g19202.dmakogon.factory.items.Motor;
import ru.nsu.fit.g19202.dmakogon.threadpool.Task;

public class WorkerTask implements Task
{
    private final Logger logger = LogManager.getLogger();

    private final String name;
    private final Storage<Motor> motorsStorage;
    private final Storage<Body> bodiesStorage;
    private final Storage<Accessory> accessoriesStorage;
    private final Storage<Car> carsStorage;

    public WorkerTask(String name, Storage<Motor> motorsStorage, Storage<Body> bodiesStorage, Storage<Accessory> accessoriesStorage, Storage<Car> carsStorage)
    {
        this.name = name;
        this.motorsStorage = motorsStorage;
        this.bodiesStorage = bodiesStorage;
        this.accessoriesStorage = accessoriesStorage;
        this.carsStorage = carsStorage;
    }

    @Override
    public String getName()
    {
        return name;
    }

    @Override
    public void performWork() throws InterruptedException
    {
        Body body = bodiesStorage.getItem();
        Accessory accessory = accessoriesStorage.getItem();
        Motor motor = motorsStorage.getItem();
        Car car = new Car(body, accessory, motor);
        carsStorage.putItem(car);
        logger.info("Produced car " + car);
    }
}
