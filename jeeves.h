#ifndef JEEVES_H
#define JEEVES_H

#include <QCoreApplication>
#include <QtNetwork>
#include <QTimer>

#include "locator.h"

class Jeeves : public QCoreApplication
{
    Q_OBJECT

public:
    Jeeves(QHostAddress broadcastAddress, int &argc, char **argv);

public slots:
    void OnJenkinsInstanceRefresh();

private:
    Locator * m_locator;
    QTimer * m_timer;
};

#endif // JEEVES_H
