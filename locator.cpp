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

#include <QtNetwork>
#include <QTextStream>
#include <iostream>

#include "locator.h"
#include "network.h"
#include "log.h"

using namespace std;

Locator::Locator(QHostAddress & broadcastAddress, QObject *parent) :
    QObject(parent),
    m_broadcastAddress(broadcastAddress)
{
}

void Locator::run()
 {
    Log::Instance()->Status(QString("Broadcasting on %1 (UDP port %2)...").arg(m_broadcastAddress.toString()).arg(Jenkins_UDP));

    m_udpSocket = new QUdpSocket();

    m_udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    QByteArray message = "Where are all the build machines ?";
    m_udpSocket->writeDatagram(message.data(),message.size(), m_broadcastAddress , Jenkins_UDP );
}

void Locator::readPendingDatagrams()
{
    cout << "Waiting for a response..." << std::endl;
    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender);
        m_builders[sender.toString()] = QString(datagram);

        Log::Instance()->Status(QString("Jenkins instance discovered on: %1").arg(sender.toString()));
        Log::Instance()->Status(QString("Reply:%1").arg(QString(datagram)));
    }
    delete m_udpSocket;
    emit finished();
}



