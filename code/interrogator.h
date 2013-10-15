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

#ifndef INTERROGATOR_H
#define INTERROGATOR_H

#include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

#include "builds.h"

class Interrogator : QObject
{
    Q_OBJECT

public:
    Interrogator(Builds * builds, QObject * parent = 0);
    ~Interrogator();

    void Request(QStringList apiList);
    void Request(QString api);

public slots:
    void OnRequestReceived(QNetworkReply * reply);
    void ParseHudsonResponse(QXmlStreamReader & xml);
    void ParseProjectResponse(QXmlStreamReader & xml);
    void ParseBuildResponse(QXmlStreamReader & xml);

private:
    QNetworkAccessManager * m_accessManager;
    Builds * m_builds;
};

#endif // INTERROGATOR_H
