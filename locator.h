#ifndef LOCATOR_H
#define LOCATOR_H

#include <QObject>
#include <QMap>
#include <QtNetwork>

class Locator : public QObject
{
    Q_OBJECT
public:
    explicit Locator(QHostAddress & broadcastAddress, QObject *parent = 0);

private:
    QUdpSocket *            m_udpSocket;
    QMap<QString, QString>  m_builders;
    QHostAddress            m_broadcastAddress;

signals:
    void finished();

public slots:
    void run();
    void readPendingDatagrams();
 };

#endif // LOCATOR_H
