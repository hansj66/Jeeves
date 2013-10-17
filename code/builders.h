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

#ifndef BUILDERS_H
#define BUILDERS_H

#include <QObject>
#include <QString>
#include <QTimer>
#include "builder.h"
#include "FileDownloader.h"




class Builders : public QObject
{
    Q_OBJECT
public:
    Builders();
    Builds builds();
    Build *build(QString url) ;
    int RemoveStale();


public slots:
    void Add(QString url);
    int  Count() { return m_builders.count();}
    void OnAddBuilders(QStringList urls);
    void OnRemoveBuilders(QStringList urls);


private:
    QString rootUrl(QString xml);
    QString tagData(QString tag);

    QString m_API;
    QString m_XML;
    QString m_Target;
    bool m_valid;

    FileDownloader * m_fileDownloader;
    QTimer      * m_refreshTimer;

    QMap<QString, Builder> m_builders;

private slots:
    void refreshBuilds();

};

#endif // BUILDER_H
