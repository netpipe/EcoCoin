#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

#include "ui_chatwidget.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget, Ui::ChatWidget
{
    Q_OBJECT

    QString         m_nickname;
    QStringList     m_messages;

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void            rebuildHistory();

signals:
    void            message(const QString &nickname, const QString &text);
    void            action(const QString &nickname, const QString &text);

private slots:
    void            messageSlot(const QString &nickname, const QString &text);
    void            actionSlot(const QString &nickname, const QString &text);
    void            textChangedSlot(const QString &newText);
    void            setNameClickedSlot();
    void            sendClickedSlot();

private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
