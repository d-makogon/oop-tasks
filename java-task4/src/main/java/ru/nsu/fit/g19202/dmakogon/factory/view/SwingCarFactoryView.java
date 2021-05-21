package ru.nsu.fit.g19202.dmakogon.factory.view;

import ru.nsu.fit.g19202.dmakogon.factory.CarFactory;
import ru.nsu.fit.g19202.dmakogon.factory.CarFactoryObserver;
import ru.nsu.fit.g19202.dmakogon.factory.FactorySettings;
import ru.nsu.fit.g19202.dmakogon.factory.FactorySettingsObserver;
import ru.nsu.fit.g19202.dmakogon.factory.items.Accessory;
import ru.nsu.fit.g19202.dmakogon.factory.items.Body;
import ru.nsu.fit.g19202.dmakogon.factory.items.Car;
import ru.nsu.fit.g19202.dmakogon.factory.items.Motor;

import javax.swing.*;
import java.awt.*;

public class SwingCarFactoryView implements FactorySettingsObserver, FactoryView, CarFactoryObserver
{
    private static final int MIN_MSECS = 100;
    private static final int MAX_MSECS = 3100;
    private static final int SLIDER_STEP = 1000;
    private final CarFactory carFactory;
    private final JFrame frame;
    private final NamedLabel accessoryProduceTimeLabel;
    private final NamedLabel bodyProduceTimeLabel;
    private final NamedLabel motorProduceTimeLabel;
    private final NamedLabel dealerDelayLabel;
    private final NamedLabel dealersCountLabel;
    private final NamedLabel accessoriesSuppliersCountLabel;

    private final NamedLabel curBodiesCount;
    private final NamedLabel curAccessoriesCount;
    private final NamedLabel curMotorsCount;
    private final NamedLabel curCarsCount;
    private final NamedLabel totalCarsCount;

    private final JSlider dealerDelaySlider;
    private final JSlider motorProduceTimeSlider;
    private final JSlider bodyProduceTimeSlider;
    private final JSlider accessoryProduceTimeSlider;

    public SwingCarFactoryView(CarFactory carFactory)
    {
        this.carFactory = carFactory;
        FactorySettings factorySettings = carFactory.getFactorySettings();

        frame = new JFrame("Car Factory");

        dealerDelaySlider = createTimeSlider(factorySettings.getDealerDelay());
        motorProduceTimeSlider = createTimeSlider(factorySettings.getMotorProduceTime());
        bodyProduceTimeSlider = createTimeSlider(factorySettings.getBodyProduceTime());
        accessoryProduceTimeSlider = createTimeSlider(factorySettings.getAccessoryProduceTime());

        dealerDelayLabel = new NamedLabel("Dealer Delay");
        dealersCountLabel = new NamedLabel("Dealers Count");
        dealersCountLabel.setText(Integer.toString(factorySettings.getDealersCount()));
        motorProduceTimeLabel = new NamedLabel("Motor Produce Time");
        bodyProduceTimeLabel = new NamedLabel("Body Produce Time");
        accessoryProduceTimeLabel = new NamedLabel("Accessory Produce Time");
        accessoriesSuppliersCountLabel = new NamedLabel("Accessories Suppliers Count");
        accessoriesSuppliersCountLabel.setText(Integer.toString(factorySettings.getAccessoriesSuppliersCount()));
        curBodiesCount = new NamedLabel("Current Bodies");
        curAccessoriesCount = new NamedLabel("Current Accessories");
        curMotorsCount = new NamedLabel("Current Motors");
        curCarsCount = new NamedLabel("Current Cars");
        totalCarsCount = new NamedLabel("Total Cars");

        dealerDelaySlider.addChangeListener((x) -> factorySettings.setDealerDelay(dealerDelaySlider.getValue()));
        motorProduceTimeSlider.addChangeListener((x) -> factorySettings.setMotorProduceTime(motorProduceTimeSlider.getValue()));
        bodyProduceTimeSlider.addChangeListener((x) -> factorySettings.setBodyProduceTime(bodyProduceTimeSlider.getValue()));
        accessoryProduceTimeSlider.addChangeListener((x) -> factorySettings.setAccessoryProduceTime(accessoryProduceTimeSlider.getValue()));


        Container contentPane = frame.getContentPane();
        contentPane.setLayout(new GridBagLayout());

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.insets = new Insets(3, 5, 3, 5);
        addTwoComponents(contentPane, gbc, dealerDelayLabel, dealerDelaySlider);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, dealersCountLabel);
        addSeparator(contentPane, gbc);
        addTwoComponents(contentPane, gbc, motorProduceTimeLabel, motorProduceTimeSlider);
        addSeparator(contentPane, gbc);
        addTwoComponents(contentPane, gbc, bodyProduceTimeLabel, bodyProduceTimeSlider);
        addSeparator(contentPane, gbc);
        addTwoComponents(contentPane, gbc, accessoryProduceTimeLabel, accessoryProduceTimeSlider);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, accessoriesSuppliersCountLabel);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, curBodiesCount);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, curAccessoriesCount);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, curMotorsCount);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, curCarsCount);
        addSeparator(contentPane, gbc);
        addComponent(contentPane, gbc, totalCarsCount);
    }

    private static void addSeparator(Container container, GridBagConstraints gbc)
    {
        JSeparator separator = new JSeparator(JSeparator.HORIZONTAL);
        separator.setBackground(Color.GRAY);
        separator.setForeground(Color.GRAY);
        gbc.gridx = 0;
        gbc.gridwidth = 2;
        container.add(separator, gbc);
        gbc.gridy++;
    }

    private static void addComponent(Container container, GridBagConstraints gbc, JComponent component)
    {
        gbc.gridx = 0;
        gbc.gridwidth = 1;
        container.add(component, gbc);
        gbc.gridy++;
    }

    private static void addTwoComponents(Container container, GridBagConstraints gbc, JComponent component1,
                                         JComponent component2)
    {
        gbc.gridx = 0;
        gbc.gridwidth = 1;
        container.add(component1, gbc);
        gbc.gridx = 1;
        container.add(component2, gbc);
        gbc.gridy++;
    }

    private JSlider createTimeSlider(int value)
    {
        JSlider slider = new JSlider(MIN_MSECS, MAX_MSECS, value);
        slider.setPaintLabels(true);
        slider.setMajorTickSpacing(SLIDER_STEP);
        slider.setMinorTickSpacing(10);
        slider.setSnapToTicks(true);
        return slider;
    }

    @Override
    public void accessoryProduceTimeChanged(FactorySettings settings, int newValue)
    {
        accessoryProduceTimeLabel.setText(Integer.toString(newValue));
        frame.repaint();
    }

    @Override
    public void bodyProduceTimeChanged(FactorySettings settings, int newValue)
    {
        bodyProduceTimeLabel.setText(Integer.toString(newValue));
        frame.repaint();
    }

    @Override
    public void motorProduceTimeChanged(FactorySettings settings, int newValue)
    {
        motorProduceTimeLabel.setText(Integer.toString(newValue));
        frame.repaint();
    }

    @Override
    public void dealerDelayChanged(FactorySettings settings, int newValue)
    {
        dealerDelayLabel.setText(Integer.toString(newValue));
        frame.repaint();
    }

    private void runImpl()
    {
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setResizable(false);
        frame.setLocationRelativeTo(null);
        frame.pack();
        frame.setVisible(true);
    }

    @Override
    public void run()
    {
        SwingUtilities.invokeLater(this::runImpl);
    }

    @Override
    public void bodyAdded(CarFactory carFactory, Body body)
    {
        curBodiesCount.setText(Integer.toString(carFactory.getBodiesStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void bodyRemoved(CarFactory carFactory, Body body)
    {
        curBodiesCount.setText(Integer.toString(carFactory.getBodiesStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void motorAdded(CarFactory carFactory, Motor motor)
    {
        curMotorsCount.setText(Integer.toString(carFactory.getMotorsStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void motorRemoved(CarFactory carFactory, Motor motor)
    {
        curMotorsCount.setText(Integer.toString(carFactory.getMotorsStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void accessoryAdded(CarFactory carFactory, Accessory accessory)
    {
        curAccessoriesCount.setText(Integer.toString(carFactory.getAccessoriesStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void accessoryRemoved(CarFactory carFactory, Accessory accessory)
    {
        curAccessoriesCount.setText(Integer.toString(carFactory.getAccessoriesStorage().getCurrentSize()));
        frame.repaint();
    }

    @Override
    public void carAdded(CarFactory carFactory, Car car)
    {
        curCarsCount.setText(Integer.toString(carFactory.getCarsStorage().getCurrentSize()));
        totalCarsCount.setText(Integer.toString(carFactory.getTotalProducedCarsCount()));
        frame.repaint();
    }

    @Override
    public void carRemoved(CarFactory carFactory, Car car)
    {
        curCarsCount.setText(Integer.toString(carFactory.getCarsStorage().getCurrentSize()));
        totalCarsCount.setText(Integer.toString(carFactory.getTotalProducedCarsCount()));
        frame.repaint();
    }
}
