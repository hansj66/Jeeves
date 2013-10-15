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
#include "builder.h"
#include "log.h"

Builder::Builder(QString xml, QObject * parent)
    : QObject(parent),
      m_XML(xml)
{
    m_valid = true;
    tagData(xml);
}

QString Builder::API()
{
    return QString("%1api/xml/").arg(rootUrl(tagData("url")));
}


QString Builder::rootUrl(QString url)
{
    return url.left(url.lastIndexOf("/")+1);
}

QString Builder::tagData(QString tag)
{
    QDomDocument doc;
    if (!doc.setContent(m_XML))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine URL : %1 - is complete garbage.").arg(m_XML));
        return QString();
    }

    QDomElement root = doc.documentElement();
    QDomNodeList elements = root.elementsByTagName("url");
    if (1 != elements.count())
    {
        Log::Instance()->Error(QString("Dang ! I was hoping for a single element. Instead we got %1. This probably should not happen...").arg(elements.count()));
        m_valid = false;
        return QString();
    }

    return elements.at(0).toElement().text();
}

