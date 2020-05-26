#include "chatwidget.h"
#include "ui_chatwidget.h"

#include "chat_adaptor.h"
#include "chat_interface.h"


ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    setupUi(this);

    bool connected = QDBusConnection::sessionBus().isConnected();

    this->setEnabled(connected);

    btnSend->setEnabled(false);

    connect(editMessage, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedSlot(QString)));
    connect(btnSetName, SIGNAL(clicked(bool)), this, SLOT(setNameClickedSlot()));
    connect(btnSend, SIGNAL(clicked(bool)), this, SLOT(sendClickedSlot()));
    connect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(exiting()));

    // add our D-Bus interface and connect to D-Bus
    new ChatAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);

    org::example::chat *iface;
    iface = new org::example::chat(QString(), QString(), QDBusConnection::sessionBus(), this);
    //connect(iface, SIGNAL(message(QString,QString)), this, SLOT(messageSlot(QString,QString)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "org.qtcoin.chat", "message", this, SLOT(messageSlot(QString,QString)));
    connect(iface, SIGNAL(action(QString,QString)), this, SLOT(actionSlot(QString,QString)));

    m_nickname = QString("anonymous");
    editName->setText(m_nickname);
    emit action(m_nickname, QLatin1String("joins the chat"));

}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::rebuildHistory()
{
    QString history = m_messages.join( QLatin1String("\n" ) );
    chatHistory->setPlainText(history);
}

void ChatWidget::messageSlot(const QString &nickname, const QString &text)
{
    QString msg( QLatin1String("<%1> %2") );
    msg = msg.arg(nickname, text);
    m_messages.append(msg);

    if (m_messages.count() > 100)
        m_messages.removeFirst();
    rebuildHistory();
}

void ChatWidget::actionSlot(const QString &nickname, const QString &text)
{
    QString msg( QLatin1String("* %1 %2") );
    msg = msg.arg(nickname, text);
    m_messages.append(msg);

    if (m_messages.count() > 100)
        m_messages.removeFirst();
    rebuildHistory();
}

void ChatWidget::textChangedSlot(const QString &newText)
{
    btnSend->setEnabled(!newText.isEmpty());
}

void ChatWidget::sendClickedSlot()
{
    //emit message(m_nickname, messageLineEdit->text());
    QDBusMessage msg = QDBusMessage::createSignal("/", "org.qtcoin.chat", "message");
    msg << m_nickname << editMessage->text();
    QDBusConnection::sessionBus().send(msg);
    editMessage->setText(QString());
}

void ChatWidget::setNameClickedSlot()
{
    m_nickname = editName->text().trimmed();
    emit action(m_nickname, QLatin1String("changed name."));
}
