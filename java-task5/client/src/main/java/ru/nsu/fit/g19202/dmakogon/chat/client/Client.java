package ru.nsu.fit.g19202.dmakogon.chat.client;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.chat.Message;
import ru.nsu.fit.g19202.dmakogon.chat.MessageSerializer;

import java.io.*;
import java.net.Socket;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

public class Client implements Runnable
{
    private static final Logger logger = LogManager.getLogger();

    private final Socket socket;

    private final InputStream socketIn;
    private final OutputStream socketOut;
    private final MessageSerializer messageSerializer;
    private final AtomicBoolean loggedOut = new AtomicBoolean(false);
    private final CopyOnWriteArrayList<ClientObserver> observers = new CopyOnWriteArrayList<>();
    private final ArrayBlockingQueue<String> messagesQueue = new ArrayBlockingQueue<>(100);
    private final Thread inputThread = new Thread(this::inputLoop);
    private final String name;
    private int id = 0;
    private final Thread outputThread = new Thread(this::outputLoop);

    public Client(String name, Socket socket, MessageSerializer messageSerializer) throws IOException
    {
        this.socket = socket;
        this.name = name;
        this.messageSerializer = messageSerializer;
        socketIn = socket.getInputStream();
        socketOut = socket.getOutputStream();
    }

    public void addObserver(ClientObserver obs)
    {
        observers.add(obs);
    }

    public void removeObserver(ClientObserver obs)
    {
        observers.remove(obs);
    }

    // handles messages from server
    private void inputLoopImpl() throws IOException
    {
        while (!loggedOut.get())
        {
            Message receivedMessage = messageSerializer.readMessage(socketIn);

            switch (receivedMessage.getType())
            {
                case ERROR -> {
                    logger.error("Server sent error message: " + receivedMessage.getErrorMessage());
                }
                case LIST_SUCCESS -> {
                    logger.info("Server sent users list");
                }
                case MESSAGE_IN -> {
                    logger.info("Received text message from " + receivedMessage.getClientName());
                }
                case USER_LOGIN_EVENT -> {
                    logger.info(receivedMessage.getClientName() + " has connected");
                }
                case USER_LOGOUT_EVENT -> {
                    logger.info(receivedMessage.getClientName() + " has left");
                }
                default -> {
                    logger.error("Unexpected message type: " + receivedMessage.getType());
                    return;
                }
            }

            observers.forEach((obs) -> obs.onMessageReceive(this, receivedMessage));
        }
    }

    public void sendMessage(String message) throws InterruptedException
    {
        messagesQueue.put(message);
    }

    // handles input from user
    private void outputLoopImpl() throws IOException
    {
        while (true)
        {
            String userInput;
            try
            {
                userInput = messagesQueue.take();
            }
            catch (InterruptedException e)
            {
                logout();
                break;
            }

            if (userInput.equalsIgnoreCase("quit"))
            {
                logout();
                break;
            }
            else if (userInput.equalsIgnoreCase("list"))
            {
                // set UserMessageHandler for each message user can type

                Message listMessage = Message.createListMessage(id);
                messageSerializer.writeMessage(socketOut, listMessage);
            }
            else
            {
                Message textMessage = Message.createOutTextMessage(id, userInput);
                messageSerializer.writeMessage(socketOut, textMessage);
            }
        }
    }

    private void logout() throws IOException
    {
        logger.info("Logging out...");
        loggedOut.set(true);

        Message logoutMessage = Message.createLogoutMessage(id);
        messageSerializer.writeMessage(socketOut, logoutMessage);
    }

    private void inputLoop()
    {
        try
        {
            inputLoopImpl();
        }
        catch (IOException e)
        {
            loggedOut.set(true);
            logger.error(e.getMessage());
        }
    }

    private void outputLoop()
    {
        try
        {
            outputLoopImpl();
        }
        catch (IOException e)
        {
            logger.error(e.getMessage());
        }
    }

    @Override
    public void run()
    {
        logger.info("Logging in chat...");

        try
        {
            if (!login())
            {
                logger.error("Couldn't login");
                return;
            }
        }
        catch (IOException e)
        {
            logger.error(e.getMessage());
            return;
        }

        inputThread.start();
        outputThread.start();
    }

    public void stop()
    {
        inputThread.interrupt();
        outputThread.interrupt();

        try
        {
            inputThread.join();
            outputThread.join();
            socket.close();
        }
        catch (InterruptedException e)
        {
            logger.error("Error while joining threads: " + e.getMessage());
        }
        catch (IOException e)
        {
            logger.error("Error while closing socket: " + e.getMessage());
        }

        observers.forEach(obs -> obs.onStop(this));
    }

    private boolean login() throws IOException
    {
        logger.info("Trying to login...");

        Message loginMessage = Message.createLoginMessage(name);
        messageSerializer.writeMessage(socketOut, loginMessage);

        Message msgFromServer = messageSerializer.readMessage(socketIn);
        var msgType = msgFromServer.getType();

        if (msgType == Message.Type.LOGIN_SUCCESS)
        {
            id = msgFromServer.getClientId();
            // userOutPrint.println("Login successful! Your id is " + id);
            logger.info("Login successful! Id=" + id);
            observers.forEach(obs -> obs.onLoginSuccess(this));
        }
        else if (msgType == Message.Type.ERROR)
        {
            logger.error("Login error: " + msgFromServer.getErrorMessage());
            observers.forEach(obs -> obs.onLoginFailure(this));
            return false;
        }

        return true;
    }
}
