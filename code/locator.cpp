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
    m_udpSocket(0),
    m_broadcastAddress(broadcastAddress)
{
    run();
    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, SIGNAL(timeout()), this, SLOT(run()));
    m_broadcastTimer->start(60000);

}

void Locator::clear()
{
    delete m_udpSocket;
    m_udpSocket = 0L;

}

void Locator::run()
 {
    clear();
    Log::Instance()->Status(QString("Broadcasting on %1 (UDP port %2)...").arg(m_broadcastAddress.toString()).arg(Jenkins_UDP));

    if (0L != m_udpSocket)
        delete m_udpSocket;
    m_udpSocket = new QUdpSocket();

    m_udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    QByteArray message = "Where are all the build machines ?";
    m_udpSocket->writeDatagram(message.data(),message.size(), m_broadcastAddress , Jenkins_UDP );
 }


void Locator::CheckForBuilderChanges(QSet<QString> buildMachineURLs)
{
    if(buildMachineURLs.isEmpty()) return;
    QSet<QString> newBuilds;
    QSet<QString> disapearedBuilds;

    disapearedBuilds = m_knownBuildMachines - buildMachineURLs;
    if(!disapearedBuilds.isEmpty())
        emit buildersDisapeared(disapearedBuilds.toList());

    newBuilds = buildMachineURLs - m_knownBuildMachines;
    if(!newBuilds.isEmpty())
        emit buildersFound(newBuilds.toList());

    m_knownBuildMachines = buildMachineURLs;

}

void Locator::readPendingDatagrams()
{
    cout << "Waiting for a response..." << std::endl;

    QSet<QString> builderUrls;

    for (int i=0; i< 10; i++)
    {
        while (m_udpSocket->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(m_udpSocket->pendingDatagramSize());
            QHostAddress sender;
            m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender);
            QString datagramString = QString(datagram);
            QString url = GetUrl(datagramString);

            //Log::Instance()->Status(QString("Jenkins discovered on:   %1").arg(sender.toString()));

            if(!url.isEmpty())
            {
                builderUrls << url;
                Log::Instance()->Status(QString("Url                  :   %1").arg(url));
            }
        }
    }
    CheckForBuilderChanges(builderUrls);
}

QString Locator::GetUrl(const QString &datagram) const
{
    QDomDocument doc;
    if (!doc.setContent(datagram))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine URL : %1 - is complete garbage.").arg(datagram));
        return QString();
    }

    QDomElement root = doc.documentElement();
    QDomNodeList elements = root.elementsByTagName("url");
    if (1 != elements.count())
    {
       // Log::Instance()->Error(QString("Dang ! I was hoping for a single element. Instead we got %1. This probably should not happen...").arg(elements.count()));
        return QString();
    }
    return QString("%1api/xml/").arg(elements.at(0).toElement().text());
}



