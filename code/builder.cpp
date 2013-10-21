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
#include "builder.h"
#include "log.h"

Builder::Builder()
{

}

Builder::Builder(const QByteArray &xml)
{
    parseXml(xml);
}

bool Builder::parseXml(const QByteArray &xml)
{
    clear();
    QDomDocument doc;
    if (!doc.setContent(xml))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine is complete garbage."));
        return false;
    }
    QDomElement root = doc.documentElement();
    if(root.tagName() != "hudson")
        return false;

    QDomNode nodeParent = root.firstChild();
    while(!nodeParent.isNull())
    {
        QDomElement element = nodeParent.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == "job")
            {
                QDomNode jobNode = nodeParent.firstChild();
                Build * build = new Build(jobNode);
                m_builds.append(build);
            }
        }
        nodeParent = nodeParent.nextSibling();
    }
    return true;
}



Builds Builder::builds() const
{
    return m_builds;
}

void Builder::clear()
{
    foreach(Build * b, m_builds)
    {
        delete b;
    }
    m_builds.clear();

}

int Builder::RemoveStale()
{
    int removed = 0;
    foreach(Build * b, m_builds)
    {
        if(b->LastHeardFrom().addSecs(300) <  QDateTime::currentDateTime())
        {
           m_builds.removeOne(b);
           removed++;
           if(b)
           {
               delete b;
               b = 0L;
           }
        }
    }
    return removed;
}

