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

#include <QDomDocument>
#include <QDebug>
#include <QStringList>

#include <QtNetwork>
#include "builders.h"
#include "log.h"

Builders::Builders()
{
    m_fileDownloader = new FileDownloader(this);
    connect(m_fileDownloader, SIGNAL(downloaded(QString)),this, SLOT(Add(QString)));

    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(refreshBuilds()));
    m_refreshTimer->start(3000);
}


Builds Builders::builds()
{
    Builds builds;
    QMapIterator<QString, Builder> i(m_builders);
    while (i.hasNext())
    {
        i.next();
        builds.append(i.value().builds());
    }
    return builds;
}

Build* Builders::build(QString url)
{
    QMapIterator<QString, Builder> i(m_builders);
    while (i.hasNext())
    {
        i.next();
        foreach(Build * b, i.value().builds())
        {
            if(b->Url().replace("%20", " ") == url)
                return b;
        }

    }
    return 0L;
}

void Builders::Add(QString url)
{
    QString xml =  QString(m_fileDownloader->downloadedData(url));
    if(!xml.isEmpty())
    {
        QXmlStreamReader xmlReader(xml);
        xmlReader.readNextStartElement();
        if(xmlReader.name().endsWith("hudson"))
        {
            m_builders[url] = Builder(xml);
            foreach(Build * b, m_builders[url].builds())
            {
                if(!b->Url().isEmpty())
                    m_fileDownloader->Get(b->Url());
            }
        }
        else if(xmlReader.name().endsWith("Project") )
        {
            Build * b = build(url);
            if(b)
                b->parseXml(xml);
        }

    }

}

void Builders::OnAddBuilders(QStringList urls)
{
    foreach(QString url, urls)
    {
        m_fileDownloader->Get(url);
        Log::Instance()->Status(url, QString("Adding: "));
    }
}

void Builders::OnRemoveBuilders(QStringList urls)
{
    foreach(QString url, urls)
    {
        if(m_builders.contains(url))
            m_builders.remove(url);
    }
}


void Builders::refreshBuilds()
{
    foreach(Build * b, builds())
    {
        m_fileDownloader->Get(b->Url());
    }
}

int Builders::RemoveStale()
{
    int removed = 0;
    QMapIterator<QString, Builder> i(m_builders);
    while (i.hasNext())
    {
        i.next();
        removed += m_builders[i.key()].RemoveStale();
    }
    return removed;
}



