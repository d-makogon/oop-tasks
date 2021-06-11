package ru.nsu.fit.g19202.dmakogon.chat.client;

import ru.nsu.fit.g19202.dmakogon.chat.Message;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.concurrent.CopyOnWriteArrayList;

public class ChatView
{
    private final CopyOnWriteArrayList<ChatViewObserver> observers = new CopyOnWriteArrayList<>();
    private final JTextField textField;
    private final JButton sendButton;
    private final JPanel messagesPanel;
    private final JScrollPane scrollPane;

    public ChatView(JFrame parent)
    {
        messagesPanel = new JPanel();
        scrollPane = new JScrollPane(messagesPanel);
        messagesPanel.setLayout(new BoxLayout(messagesPanel, BoxLayout.Y_AXIS));
        scrollPane.setPreferredSize(new Dimension(400, 800));
        scrollPane.getVerticalScrollBar().addAdjustmentListener(e -> e.getAdjustable().setValue(e.getAdjustable().getMaximum()));

        textField = new JTextField();
        textField.addKeyListener(new KeyAdapter()
        {
            @Override
            public void keyPressed(KeyEvent e)
            {
                if (e.getKeyCode() == KeyEvent.VK_ENTER)
                {
                    sendText();
                }
            }
        });
        sendButton = new JButton("Send");
        sendButton.addActionListener(e -> sendText());
        parent.getContentPane().setLayout(new BorderLayout());
        parent.getContentPane().add(scrollPane, BorderLayout.NORTH);
        parent.getContentPane().add(textField, BorderLayout.CENTER);
        parent.getContentPane().add(sendButton, BorderLayout.SOUTH);
        // parent.setPreferredSize(new Dimension(400, 800));
        textField.requestFocus();
    }

    private void sendText()
    {
        String text = textField.getText();
        textField.setText("");
        textField.requestFocus();
        observers.forEach(obs -> obs.onUserInput(ChatView.this, text));
    }

    public void addObserver(ChatViewObserver obs)
    {
        observers.add(obs);
    }

    public void removeObserver(ChatViewObserver obs)
    {
        observers.remove(obs);
    }

    public void addMessage(Message message)
    {
        switch (message.getType())
        {
            case ERROR -> {
                messagesPanel.add(new JLabel("Error: " + message.getErrorMessage()));
            }
            case LIST_SUCCESS -> {
                StringBuilder listStringBuilder = new StringBuilder();
                message.getUsersList().forEach(s -> listStringBuilder.append(s).append("<br>"));
                messagesPanel.add(new JLabel("<html>Users list: <br>" + listStringBuilder + "</html>"));
            }
            case MESSAGE_IN -> {
                messagesPanel.add(new JLabel(String.format("Message from %s: %s%n", message.getClientName(),
                        message.getMessage())));
            }
            case USER_LOGIN_EVENT -> {
                messagesPanel.add(new JLabel(message.getClientName() + " has connected"));
            }
            case USER_LOGOUT_EVENT -> {
                messagesPanel.add(new JLabel(message.getClientName() + " has left"));
            }
        }
        messagesPanel.repaint();
        scrollPane.revalidate();
    }
}
