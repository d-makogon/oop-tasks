package ru.nsu.fit.g19202.dmakogon.factory;

import com.google.gson.Gson;

import java.io.*;
import java.net.URL;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

public class FactorySettings
{
    private static transient final Object lock = new Object();
    private static transient final String SETTINGS_FILE = "factorySettings.json";
    private final int bodiesStorageSize;
    private final int motorsStorageSize;
    private final int accessoriesStorageSize;
    private final int carsStorageSize;
    private final int accessoriesSuppliersCount;
    private final int workersCount;
    private final int dealersCount;
    private final AtomicInteger accessoryProduceTime;
    private final AtomicInteger bodyProduceTime;
    private final AtomicInteger motorProduceTime;
    private final AtomicInteger dealerDelay;
    private transient CopyOnWriteArrayList<FactorySettingsObserver> observers = new CopyOnWriteArrayList<>();

    public FactorySettings(int bodiesStorageSize, int motorsStorageSize, int accessoriesStorageSize,
                           int carsStorageSize, int accessoriesSuppliersCount, int workersCount, int dealersCount,
                           int accessoryProduceTime, int bodyProduceTime, int motorProduceTime, int dealerDelay)
    {
        this.bodiesStorageSize = bodiesStorageSize;
        this.motorsStorageSize = motorsStorageSize;
        this.accessoriesStorageSize = accessoriesStorageSize;
        this.carsStorageSize = carsStorageSize;
        this.accessoriesSuppliersCount = accessoriesSuppliersCount;
        this.workersCount = workersCount;
        this.dealersCount = dealersCount;
        this.accessoryProduceTime = new AtomicInteger(accessoryProduceTime);
        this.bodyProduceTime = new AtomicInteger(bodyProduceTime);
        this.motorProduceTime = new AtomicInteger(motorProduceTime);
        this.dealerDelay = new AtomicInteger(dealerDelay);
    }

    public static FactorySettings readSettings() throws IOException
    {
        synchronized (lock)
        {
            try (InputStream stream = FactorySettings.class.getResourceAsStream(SETTINGS_FILE))
            {
                if (stream == null)
                {
                    throw new FileNotFoundException("Unable to find " + SETTINGS_FILE + " resource");
                }
                Gson g = new Gson();
                FactorySettings settings = g.fromJson(new InputStreamReader(stream), FactorySettings.class);
                settings.observers = new CopyOnWriteArrayList<>();
                return settings;
            }
        }
    }

    public static void saveSettings(FactorySettings factorySettings) throws IOException
    {
        synchronized (lock)
        {
            URL resource = FactorySettings.class.getResource(SETTINGS_FILE);
            if (resource == null)
            {
                throw new IOException("Unable to open " + SETTINGS_FILE + " resource");
            }
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(resource.getPath())))
            {
                Gson g = new Gson();
                g.toJson(factorySettings, FactorySettings.class, writer);
            }
        }
    }

    public int getAccessoryProduceTime()
    {
        return accessoryProduceTime.get();
    }

    public void setAccessoryProduceTime(int accessoryProduceTime)
    {
        this.accessoryProduceTime.set(accessoryProduceTime);
        notifyObservers(obs -> obs.accessoryProduceTimeChanged(this, accessoryProduceTime));
    }

    public int getBodyProduceTime()
    {
        return bodyProduceTime.get();
    }

    public void setBodyProduceTime(int bodyProduceTime)
    {
        this.bodyProduceTime.set(bodyProduceTime);
        notifyObservers(obs -> obs.bodyProduceTimeChanged(this, bodyProduceTime));
    }

    public int getMotorProduceTime()
    {
        return motorProduceTime.get();
    }

    public void setMotorProduceTime(int motorProduceTime)
    {
        this.motorProduceTime.set(motorProduceTime);
        notifyObservers(obs -> obs.motorProduceTimeChanged(this, motorProduceTime));
    }

    public int getDealerDelay()
    {
        return dealerDelay.get();
    }

    public void setDealerDelay(int dealerDelay)
    {
        this.dealerDelay.set(dealerDelay);
        notifyObservers(obs -> obs.dealerDelayChanged(this, dealerDelay));
    }

    private void notifyObservers(Consumer<FactorySettingsObserver> eventFunction)
    {
        for (FactorySettingsObserver observer : observers)
        {
            eventFunction.accept(observer);
        }
    }

    public int getBodiesStorageSize()
    {
        return bodiesStorageSize;
    }

    public int getMotorsStorageSize()
    {
        return motorsStorageSize;
    }

    public int getAccessoriesStorageSize()
    {
        return accessoriesStorageSize;
    }

    public int getCarsStorageSize()
    {
        return carsStorageSize;
    }

    public int getAccessoriesSuppliersCount()
    {
        return accessoriesSuppliersCount;
    }

    public int getWorkersCount()
    {
        return workersCount;
    }

    public int getDealersCount()
    {
        return dealersCount;
    }

    public void addObserver(FactorySettingsObserver observer)
    {
        observers.add(observer);
        notifyObservers(obs -> obs.accessoryProduceTimeChanged(this, accessoryProduceTime.get()));
        notifyObservers(obs -> obs.bodyProduceTimeChanged(this, bodyProduceTime.get()));
        notifyObservers(obs -> obs.motorProduceTimeChanged(this, motorProduceTime.get()));
        notifyObservers(obs -> obs.dealerDelayChanged(this, dealerDelay.get()));
    }

    public void removeObserver(FactorySettingsObserver observer)
    {
        observers.remove(observer);
    }
}
