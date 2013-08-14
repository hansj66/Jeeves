/*
    Copyright 2013 Hans Jørgen Grimstad

    Jeeves is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "builds.h"

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
    Builds m_builds;
    QList<QLineEdit *> m_DisplayLines;
    QWidget * m_mainWindow;

    int m_lineHeight;
    int m_discoveredBuilds;
    int m_refreshInterval;

    void RefreshLayout();
    void RefreshData();
    void InitDisplayMessage();
};

#endif // STATUSSCREEN_H
