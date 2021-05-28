package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Accessory;
import ru.nsu.fit.g19202.dmakogon.factory.items.Body;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;
import ru.nsu.fit.g19202.dmakogon.factory.items.Motor;
import ru.nsu.fit.g19202.dmakogon.threadpool.ThreadPool;

import java.util.concurrent.atomic.AtomicInteger;

public class WorkersThreadPool extends ThreadPool implements WorkersController
{
    private final Logger logger = LogManager.getLogger();

    private final Storage<Motor> motorsStorage;
    private final Storage<Body> bodiesStorage;
    private final Storage<Accessory> accessoriesStorage;
    private final Storage<Car> carsStorage;
    private final AtomicInteger nextAssemblyTaskId = new AtomicInteger(0);

    public WorkersThreadPool(Storage<Motor> motorsStorage, Storage<Body> bodiesStorage,
                             Storage<Accessory> accessoriesStorage, Storage<Car> carsStorage)
    {
        this.motorsStorage = motorsStorage;
        this.bodiesStorage = bodiesStorage;
        this.accessoriesStorage = accessoriesStorage;
        this.carsStorage = carsStorage;
    }

    @Override
    public void addCarAssemblyTask()
    {
        int taskId = nextAssemblyTaskId.getAndIncrement();
        addTask(new WorkerTask("WorkerTask " + taskId, motorsStorage, bodiesStorage, accessoriesStorage, carsStorage));
        logger.info("Added new task " + taskId);
    }
}
