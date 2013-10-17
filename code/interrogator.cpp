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

#include <QXmlStreamReader>
#include <QCoreApplication>

#include "interrogator.h"
#include "log.h"



Interrogator::Interrogator(Builds * builds, QObject * parent) :
    QObject(parent),
   m_builds(builds)
{
    m_accessManager = new QNetworkAccessManager();
    connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnRequestReceived(QNetworkReply*)));
}

Interrogator::~Interrogator()
{
    disconnect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnRequestReceived(QNetworkReply*)));
    delete m_accessManager;
    m_accessManager = NULL;
}

void Interrogator::Request(QStringList apiList)
{
    foreach (QString api, apiList)
    {
        Request(api);
    }
}

void Interrogator::Request(QString api)
{
    m_accessManager->get(QNetworkRequest(QUrl(api)));
}


void Interrogator::OnRequestReceived(QNetworkReply * reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();

        // Oh, the joys of "quick" and dirty XML parsing...
        QXmlStreamReader xml(data);
        xml.readNextStartElement();

        if (xml.name() == "hudson")
            ParseHudsonResponse(xml);
        else if (xml.name().endsWith("Project"))
            ParseProjectResponse(xml);
        else if (xml.name().endsWith("Build"))
            ParseBuildResponse(xml);
    }
    else
    {
        Log::Instance()->Error(QString("Dang ! A HTTP request (build) failed with status code : %1").arg(statusCodeV.toString()));
    }

    reply->deleteLater();
    reply->close();
}


void Interrogator::ParseHudsonResponse(QXmlStreamReader & xml)
{
    QString name;
    QString url;

    while (!xml.atEnd())
    {
        xml.readNextStartElement();
        if (xml.name() == "job")
        {
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement &&  xml.name() == "job"))
            {
                if (xml.name() == "name")
                {
                    name = xml.readElementText();
                }
                else if (xml.name() == "url")
                {
                    url = xml.readElementText();
                }
                xml.readNext();
            }
            m_builds->UpdateLastHeardFrom(url, QDateTime::currentDateTime());
            m_builds->UpdateName(url, name);
            m_builds->UpdateUrl(url, url);
            Request(QString("%1api/xml").arg(url));
        }
    }
}

void Interrogator::ParseProjectResponse(QXmlStreamReader & xml)
{
    QString number;
    QString url;
    QString buildable;
    QString rootUrl;
    QString description;
    bool descriptionRead = false;
    bool descriptionUpdated = false;

    while (!xml.atEnd())
    {
        if ((xml.name() == "description") && (!descriptionRead))
        {
            description = xml.readElementText();
            descriptionRead = true;
        }

         xml.readNextStartElement();
        if (xml.name() == "buildable")
        {
            buildable = xml.readElementText();
        }

        else if (xml.name() == "lastBuild")
        {
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement &&  xml.name() == "lastBuild"))
            {
                if (xml.name() == "number")
                {
                    number = xml.readElementText();
                }
                else if (xml.name() == "url")
                {
                    url = xml.readElementText();
                }
                xml.readNext();

                if ((!number.isEmpty()) && (!url.isEmpty()))
                        break;
            }

            rootUrl = url.left(url.lastIndexOf("/", url.length()-2)+1);
            m_builds->UpdateNumber(rootUrl, number);
            m_builds->UpdateBuildable(rootUrl, buildable);
            Request(QString("%1api/xml").arg(url));
        }
        if (!rootUrl.isEmpty() && !description.isEmpty() && !descriptionUpdated)
        {
            m_builds->UpdateDescription(rootUrl, description);
            descriptionUpdated = true;
        }
    }
}

void Interrogator::ParseBuildResponse(QXmlStreamReader & xml)
{
    QString result;
    QString url;
    QStringList culprits;
    QString isBuilding;

    while (!xml.atEnd())
    {
        xml.readNextStartElement();
        if (xml.name() == "result")
        {
            result = xml.readElementText();
        }
        else if (xml.name() == "url")
        {
            url = xml.readElementText();
        }
        else if (xml.name() == "building")
        {
            isBuilding = xml.readElementText();
        }
        else if (xml.name() == "culprit")
        {
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement &&  xml.name() == "culprit"))
            {
                if (xml.name() == "fullName")
                {
                    culprits.append(xml.readElementText());
                }
                xml.readNext();
            }
        }
    }

    if (url.isEmpty())
        return;

    QString rootUrl = url.left(url.lastIndexOf("/", url.length()-2)+1);

     m_builds->UpdateResult(rootUrl, result);
     m_builds->UpdateCulprits(rootUrl, culprits);
     m_builds->UpdateBuilding(rootUrl, isBuilding);
}


