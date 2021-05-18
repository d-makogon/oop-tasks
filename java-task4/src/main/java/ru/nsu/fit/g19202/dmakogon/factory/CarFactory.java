package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.*;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

public class CarFactory implements FactorySettingsObserver
{
    private final Logger logger = LogManager.getLogger();
    private final Object lock = new Object();

    private final FactorySettings factorySettings;
    private final Storage<Accessory> accessoriesStorage;
    private final Storage<Body> bodiesStorage;
    private final Storage<Motor> motorsStorage;
    private final Storage<Car> carsStorage;
    private final List<Supplier<Accessory>> accessoriesSuppliers;
    private final Supplier<Body> bodySupplier;
    private final Supplier<Motor> motorSupplier;
    private final List<Dealer> dealers;
    private final WorkersThreadPool workersThreadPool;
    private final CarsStorageController carsStorageController;

    private final AtomicInteger totalCarsProducedCount = new AtomicInteger(0);
    private final transient CopyOnWriteArrayList<CarFactoryObserver> observers = new CopyOnWriteArrayList<>();

    public CarFactory(FactorySettings factorySettings, ItemsFactory<Body> bodiesFactory,
                      ItemsFactory<Motor> motorsFactory, ItemsFactory<Accessory> accessoriesFactory)
    {
        this.factorySettings = factorySettings;

        this.accessoriesStorage = new Storage<>(factorySettings.getAccessoriesStorageSize());
        this.bodiesStorage = new Storage<>(factorySettings.getBodiesStorageSize());
        this.motorsStorage = new Storage<>(factorySettings.getMotorsStorageSize());
        this.carsStorage = new Storage<>(factorySettings.getCarsStorageSize());

        accessoriesStorage.addObserver(new StorageObserver<>()
        {
            @Override
            public void itemAdded(Storage<Accessory> storage, Accessory item)
            {
                notifyObservers((obs) -> obs.accessoryAdded(CarFactory.this, item));
            }

            @Override
            public void itemRemoved(Storage<Accessory> storage, Accessory item)
            {
                notifyObservers((obs) -> obs.accessoryRemoved(CarFactory.this, item));
            }
        });

        bodiesStorage.addObserver(new StorageObserver<>()
        {
            @Override
            public void itemAdded(Storage<Body> storage, Body item)
            {
                notifyObservers((obs) -> obs.bodyAdded(CarFactory.this, item));
            }

            @Override
            public void itemRemoved(Storage<Body> storage, Body item)
            {
                notifyObservers((obs) -> obs.bodyRemoved(CarFactory.this, item));
            }
        });

        motorsStorage.addObserver(new StorageObserver<>()
        {
            @Override
            public void itemAdded(Storage<Motor> storage, Motor item)
            {
                notifyObservers((obs) -> obs.motorAdded(CarFactory.this, item));
            }

            @Override
            public void itemRemoved(Storage<Motor> storage, Motor item)
            {
                notifyObservers((obs) -> obs.motorRemoved(CarFactory.this, item));
            }
        });

        carsStorage.addObserver(new StorageObserver<>()
        {
            @Override
            public void itemAdded(Storage<Car> storage, Car item)
            {
                totalCarsProducedCount.incrementAndGet();
                notifyObservers((obs) -> obs.carAdded(CarFactory.this, item));
            }

            @Override
            public void itemRemoved(Storage<Car> storage, Car item)
            {
                notifyObservers((obs) -> obs.carRemoved(CarFactory.this, item));
            }
        });

        this.accessoriesSuppliers = new ArrayList<>(factorySettings.getAccessoriesSuppliersCount());
        this.bodySupplier = new Supplier<>("Body Supplier", bodiesStorage, factorySettings.getBodyProduceTime(),
                bodiesFactory);
        this.motorSupplier = new Supplier<>("Motor Supplier", motorsStorage, factorySettings.getMotorProduceTime(),
                motorsFactory);
        this.dealers = new ArrayList<>(factorySettings.getDealersCount());

        this.workersThreadPool = new WorkersThreadPool(motorsStorage, bodiesStorage, accessoriesStorage, carsStorage);
        this.carsStorageController = new CarsStorageController(workersThreadPool, carsStorage);
    }

    public int getTotalProducedCarsCount()
    {
        return totalCarsProducedCount.get();
    }

    public FactorySettings getFactorySettings()
    {
        return factorySettings;
    }

    public Storage<Accessory> getAccessoriesStorage()
    {
        return accessoriesStorage;
    }

    public Storage<Body> getBodiesStorage()
    {
        return bodiesStorage;
    }

    public Storage<Motor> getMotorsStorage()
    {
        return motorsStorage;
    }

    public Storage<Car> getCarsStorage()
    {
        return carsStorage;
    }

    public void start()
    {
        logger.info("Starting cars factory...");

        carsStorage.addObserver(carsStorageController);

        int accessoriesSuppliersCount = factorySettings.getAccessoriesSuppliersCount();
        int accessoryProduceTime = factorySettings.getAccessoryProduceTime();
        for (int i = 0; i < accessoriesSuppliersCount; i++)
        {
            Supplier<Accessory> supplier = new Supplier<>("Accessory Supplier " + i, accessoriesStorage,
                    accessoryProduceTime, new AccessoriesFactory());
            accessoriesSuppliers.add(supplier);
            supplier.start();
        }

        bodySupplier.setItemSupplyTime(factorySettings.getBodyProduceTime());
        motorSupplier.setItemSupplyTime(factorySettings.getMotorProduceTime());
        bodySupplier.start();
        motorSupplier.start();

        int dealersCount = factorySettings.getDealersCount();
        int dealerDelay = factorySettings.getDealerDelay();
        for (int i = 0; i < dealersCount; i++)
        {
            Dealer dealer = new Dealer("Dealer " + i, carsStorage, dealerDelay);
            dealers.add(dealer);
            dealer.start();
        }

        carsStorageController.start();

        factorySettings.addObserver(this);
    }

    @Override
    public void accessoryProduceTimeChanged(FactorySettings settings, int newValue)
    {
        for (Supplier<Accessory> accessoriesSupplier : accessoriesSuppliers)
        {
            accessoriesSupplier.setItemSupplyTime(newValue);
        }
    }

    @Override
    public void bodyProduceTimeChanged(FactorySettings settings, int newValue)
    {
        if (bodySupplier != null)
        {
            bodySupplier.setItemSupplyTime(newValue);
        }
    }

    @Override
    public void motorProduceTimeChanged(FactorySettings settings, int newValue)
    {
        if (motorSupplier != null)
        {
            motorSupplier.setItemSupplyTime(newValue);
        }
    }

    @Override
    public void dealerDelayChanged(FactorySettings settings, int newValue)
    {
        for (Dealer dealer : dealers)
        {
            dealer.setItemQueryTime(newValue);
        }
    }

    private void notifyObservers(Consumer<CarFactoryObserver> eventAction)
    {
        for (CarFactoryObserver obs : observers)
        {
            eventAction.accept(obs);
        }
    }

    public void addObserver(CarFactoryObserver observer)
    {
        observers.add(observer);
    }

    public void removeObserver(CarFactoryObserver observer)
    {
        observers.remove(observer);
    }
}
