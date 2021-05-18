package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;
import ru.nsu.fit.g19202.dmakogon.threads.ManagedThread;

public class CarsStorageController extends ManagedThread implements StorageObserver<Car>
{
    private final Logger logger = LogManager.getLogger();

    private final WorkersController workersThreadPool;
    private final Storage<Car> carStorage;

    public CarsStorageController(WorkersController workersThreadPool, Storage<Car> carStorage)
    {
        this.workersThreadPool = workersThreadPool;
        this.carStorage = carStorage;
    }

    @Override
    public void run()
    {
        while (keepRunning())
        {
            if (carStorage.isEmpty())
            {
                logger.info("Car storage is empty! Queuing new car task...");
                workersThreadPool.addCarAssemblyTask();
                managedSuspend();
            }
        }
    }

    @Override
    public void itemAdded(Storage<Car> storage, Car item)
    {
        // do nothing
    }

    @Override
    public void itemRemoved(Storage<Car> storage, Car item)
    {
        managedResume();
    }
}
