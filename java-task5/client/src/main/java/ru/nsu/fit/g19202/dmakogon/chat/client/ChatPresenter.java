package ru.nsu.fit.g19202.dmakogon.chat.client;

import ru.nsu.fit.g19202.dmakogon.chat.Message;
import ru.nsu.fit.g19202.dmakogon.chat.XMLMessageSerializer;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.net.Socket;

public class ChatPresenter implements ClientObserver, ChatViewObserver
{
    private Client client;
    private ChatView chatView;
    private JFrame frame;

    private void startImpl()
    {
        frame = new JFrame("Chat");
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.addWindowListener(new WindowAdapter()
        {
            @Override
            public void windowClosing(WindowEvent e)
            {
                client.stop();
            }
        });

        String ip;
        int port;
        String dialogText = "Enter IP:port of the server";
        while (true)
        {
            String address = JOptionPane.showInputDialog(dialogText);

            String[] userInput = address.split(":");

            if (userInput.length != 2)
            {
                dialogText = "Unable to parse server address. Try again";
                continue;
            }

            ip = userInput[0];
            try
            {
                port = Integer.parseInt(userInput[1]);
            }
            catch (NumberFormatException e)
            {
                dialogText = "Entered port is not a number. Try again";
                continue;
            }
            break;
        }

        try
        {
            Socket socket = new Socket(ip, port);

            String name = JOptionPane.showInputDialog("Enter your name");

            client = new Client(name, socket, new XMLMessageSerializer());
            chatView = new ChatView(frame);
            chatView.addObserver(this);
            frame.pack();
            client.addObserver(this);
            client.run();
        }
        catch (IOException e)
        {
            JOptionPane.showConfirmDialog(frame, e.getLocalizedMessage(), "Error", JOptionPane.DEFAULT_OPTION);
            frame.dispose();
        }
    }

    public void start()
    {
        SwingUtilities.invokeLater(this::startImpl);
    }

    @Override
    public void onUserInput(ChatView view, String input)
    {
        try
        {
            client.sendMessage(input);
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void onLoginSuccess(Client client)
    {
        System.err.println("Login success");
    }

    @Override
    public void onLoginFailure(Client client)
    {
        System.err.println("Login failure");
    }

    @Override
    public void onMessageReceive(Client client, Message message)
    {
        chatView.addMessage(message);
    }

    @Override
    public void onStop(Client client)
    {
        JOptionPane.showConfirmDialog(frame, "Session has ended", "", JOptionPane.DEFAULT_OPTION);
        frame.dispose();
    }
}
