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
	m_hidden = false;
	m_status = UNKNOWN;
	m_isBuildable = false;
}


Build::Build(QDomNode node) :
    m_lastHeardFrom(QDateTime::currentDateTime())
{
    m_isBuildable = false;
	m_hidden = false;

    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == "name")
               setName(element.text());
            else if(element.tagName() == "url")
                setUrl(element.text());
            else if(element.tagName() == "buildable")
                setBuildable(element.text().toLower() == "true");
        }
        node = node.nextSibling();
    }
}


bool Build::parseLastBuildXml(const QByteArray &xmlString)
{
    if(xmlString.isEmpty())
        return false;

    QDomDocument doc;
    if (!doc.setContent(xmlString))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine is complete garbage."));
        return false;
    }

    QDomElement root = doc.documentElement();
    if(root.tagName() != "freeStyleBuild")
        return false;

    setCulprits(QStringList());
    setStartedBy(QString());
    QStringList culprits;
    QDomNode nodeParent = root.firstChild();
    while(!nodeParent.isNull())
    {
        QDomElement element = nodeParent.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == "result")
               setResult(element.text());
            else if(element.tagName() == "building" && element.text().toLower() == "true")
                setStatus(BUILDING);
            else if(element.tagName() == "number")
               setNumber(element.text());
            else if(element.tagName() == "culprit")
            {
                QDomNode culpritNode = element.firstChild();

                while(!culpritNode.isNull())
                {
                    QDomElement culpritElement = culpritNode.toElement();
                    if(culpritElement.tagName() == "fullName")
                       culprits << culpritElement.text();
                    culpritNode = culpritNode.nextSibling();
                }

            }
            else if(element.tagName() == "action")
            {
                QDomNode actionNode = element.firstChild();
                while(!actionNode.isNull())
                {
                    QDomElement actionElement = actionNode.toElement();
                    if(actionElement.tagName() == "cause")
                    {
                        QDomNode causeNode = actionElement.firstChild();
                        while(!causeNode.isNull())
                        {
                            QDomElement causeElement = causeNode.toElement();
                            if(causeElement.tagName() == "userName")
                               setStartedBy(causeElement.text());
                            causeNode = causeNode.nextSibling();
                        }

                    }
                    actionNode = actionNode.nextSibling();
                }
            }



        }
        nodeParent = nodeParent.nextSibling();    
    }
    setCulprits(culprits);
    return true;
}


bool Build::parseXml(const QByteArray & xmlString)
{
    if(xmlString.isEmpty())
        return false;

    QDomDocument doc;
    if (!doc.setContent(xmlString))
    {
        Log::Instance()->Error(QString("Bummer ! Looks like the build machine is complete garbage."));
        return false;
    }
    QDomElement root = doc.documentElement();
    if(root.tagName() != "freeStyleProject")
        return false;


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
                    if(lastBuildElement.tagName() == "url")
                       setLastBuildUrl(lastBuildElement.text());

                    lastBuildNode = lastBuildNode.nextSibling();
                }

            }
            else if(element.tagName() == "description")
                setDescription(element.text());
            else if(element.tagName() == "buildable")
            {
                setBuildable(element.text().toLower() == "true");
                if(element.text().toLower() != "true")
                    return false;
            }

        }
        nodeParent = nodeParent.nextSibling();
    }
    setLastHeardFrom(QDateTime::currentDateTime());
    return true;

}



void Build::setResult(const QString &result)
{
    if(result == "ABORTED")
    {
         setStatus(ABORTED);
    }
    else if(result == "SUCCESS")
    {
         setStatus(SUCCESS);
    }
    else if(result == "FAILURE")
    {
        setStatus(FAILURE);
    }
    else
        setStatus(UNKNOWN);
}


QString Build::ToDisplayString() const
{
    QString build = QString("%1: %2 (Build #%3) ").arg(MachineShortName()).arg(Name()).arg(m_number);
    build.replace("_"," ");

    if (m_culprits.length() == 0)
        return build;

    if(Status() == SUCCESS)
    {
        build.append("Hero");
    }
    else if(Status() == FAILURE)
    {
        build.append("Culprit");
    }
    else if(Status() == BUILDING)
    {
        build.append(StartedBy());
        return build;
    }
    else if(Status() == ABORTED)
    {
        build.append("Who did it?");
        return build;
    }

    if (m_culprits.length() > 1)
        build.append("s");

    build.append(" : ");
    for (int i=0; i<m_culprits.length(); i++)
        build.append(QString("%1, ").arg(m_culprits.at(i)));
    return build.left(build.length()-2);
}

QString Build::MachineShortName() const
{
    return m_url.right(m_url.length()-7).split("/").at(0).split(":").at(0).split(".").at(0);
}

bool Build::IsConsistent() const
{
    if ((m_name.isEmpty()) || m_url.isEmpty() || m_number.isEmpty())
            return false;
    return true;
}

void Build::setDescription(const QString &description)
{
    m_description = description;

    if (m_description.toLower().contains("[target=windows]"))
        m_target = Windows;
    else if (m_description.toLower().contains("[target=mac]"))
        m_target = Mac;
	else if (m_description.toLower().contains("[target=linux]"))
        m_target = Linux;


	if (m_description.toLower().contains("[hide=true]"))
		setHidden();





}
