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

#include <QDebug>
#include <QXmlStreamReader>
#include "build.h"
#include "log.h"

Build::Build() :
    m_lastHeardFrom(QDateTime::currentDateTime())
{
   m_isBuilding = false;
   m_isBuildable = false;
}


Build::Build(QDomNode node) :
    m_lastHeardFrom(QDateTime::currentDateTime())
{
    m_isBuilding = false;
    m_isBuildable = false;

    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == "name")
               setName(element.text());
            else if(element.tagName() == "url")
            {
                setUrl(element.text());
            }
            else if(element.tagName() == "buildable")
                setBuildable(element.text().toLower() == "true");
            else if(element.tagName() == "lastBuild")
            {
                QDomNode lastBuildNode = node.firstChild();
                while(!lastBuildNode.isNull())
                {
                    QDomElement lastBuildElement = lastBuildNode.toElement();
                    if(lastBuildElement.tagName() == "number")
                       setNumber(lastBuildElement.text());

                    lastBuildNode = lastBuildNode.nextSibling();
                }

            }

        }
        node = node.nextSibling();
    }
}


bool Build::parseXml(QString xmlString)
{
    if(xmlString.isEmpty())
        return false;

    QDomDocument doc;
    if (!doc.setContent(xmlString))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine URL : %1 - is complete garbage.").arg(xmlString));
        return false;
    }
    QDomElement root = doc.documentElement();
    if(root.tagName() != "freeStyleProject")
        return false;


    setResult("");
    setBuildable("");
    setCulprits(QStringList());

    QDomNode nodeParent = root.firstChild();
    while(!nodeParent.isNull())
    {
        QDomElement element = nodeParent.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == "lastBuild")
            {
                QDomNode lastBuildNode = nodeParent.firstChild();
                while(!lastBuildNode.isNull())
                {
                    QDomElement lastBuildElement = lastBuildNode.toElement();
                    if(lastBuildElement.tagName() == "result")
                       setResult(lastBuildElement.text());
                    else if(lastBuildElement.tagName() == "building")
                        setBuilding(lastBuildElement.text().toLower() == "true");
                    else if(lastBuildElement.tagName() == "culprit")
                    {
                        QDomNode culpritNode = lastBuildNode.firstChild();
                        while(!culpritNode.isNull())
                        {
                            QDomElement culpritElement = culpritNode.toElement();
                            QStringList culprits;
                            if(culpritElement.tagName() == "fullName")
                               culprits << culpritElement.text();
                            setCulprits(culprits);
                            culpritNode = culpritNode.nextSibling();
                        }
                    }

                    lastBuildNode = lastBuildNode.nextSibling();
                }

            }
            else if(element.tagName() == "description")
            {
                setDescription(element.text());
            }

        }

        nodeParent = nodeParent.nextSibling();
    }
    setLastHeardFrom(QDateTime::currentDateTime());
    return true;

}


QString Build::ToString() const
{
    QString build = QString("\n\n\tname=%1\n\tlast build number=%2\n\turl=%3\n\tResult=%4\n\tAlive=%5\n\tIs building=%6\n\tIs buildable=%7\n").arg(Name()).arg(m_number).arg(m_url).arg(m_result).arg(m_lastHeardFrom.toString()).arg(m_isBuilding).arg(m_isBuildable);

    if (m_culprits.length() == 0)
        return build;

    build.append("\tculprits=");
    for (int i=0; i<m_culprits.length(); i++)
        build.append(QString("%1,").arg(m_culprits.at(i)));
    return build.left(build.length()-1);
}

QString Build::ToDisplayString() const
{
    QString build = QString("%1:%2 (Build #%3) ").arg(MachineShortName()).arg(Name()).arg(m_number);

    if (m_culprits.length() == 0)
        return build;
    if(Failed())
        build.append("Culprit");
    else
        build.append("Hero");

    if (m_culprits.length() > 1)
        build.append("s");

    build.append(" : ");
    for (int i=0; i<m_culprits.length(); i++)
        build.append(QString("%1,").arg(m_culprits.at(i)));
    return build.left(build.length()-1);
}

QString Build::MachineShortName() const
{
    return m_url.right(m_url.length()-7).split("/").at(0).split(":").at(0);
}

bool Build::IsConsistent() const
{
    if ((m_name.isEmpty()) || m_url.isEmpty() || m_number.isEmpty())
            return false;
    return true;
}

void Build::setDescription(QString description)
{
    m_description = description;

    if (m_description.toLower().contains("[target=windows]"))
        m_target = Windows;
    else if (m_description.toLower().contains("[target=mac]"))
        m_target = Mac;
    if (m_description.toLower().contains("[target=linux]"))
        m_target = Linux;
}
