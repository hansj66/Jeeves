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

#ifndef LOCATOR_H
#define LOCATOR_H

#include <QObject>
#include <QtNetwork>
#include <QList>

#include "builder.h"

class Locator : public QObject
{
    Q_OBJECT
public:
    explicit Locator(QHostAddress & broadcastAddress, QObject *parent = 0);
    QStringList BuildMachineAPIs();

private:
    void clear();



    QUdpSocket *    m_udpSocket;
    QList<Builder *>  m_builders;
    QHostAddress    m_broadcastAddress;

    QString GetAPI(QString buildMachineURL);

signals:
    void finished();

public slots:
    void run();
    void readPendingDatagrams();
 };

#endif // LOCATOR_H
