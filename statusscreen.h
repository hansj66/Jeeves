#ifndef STATUSSCREEN_H
#define STATUSSCREEN_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTimer>
#include <QList>
#include <QLineEdit>

#include "log.h"
#include "interrogator.h"
#include "statusscreen.h"
#include "locator.h"

class StatusScreen : public QMainWindow
{
    Q_OBJECT
public:
    explicit StatusScreen(QHostAddress broadcastAddress, QWidget *parent = 0);
    
private:
    void InitBroadcast(QHostAddress broadcastAddress);

signals:
    
public slots:
    void OnJenkinsInstanceRefresh();

private:
    Locator * m_locator;
    Interrogator * m_interrogator;
    QTimer * m_broadcastTimer;
    QList<Build> m_builds;
    QList<QLineEdit *> m_DisplayLines;

    int m_lineHeight;
    int m_discoveredBuilds;
    int m_refreshInterval;

    void RefreshLayout();
    void RefreshData();
    void InitDisplayMessage();
};

#endif // STATUSSCREEN_H
