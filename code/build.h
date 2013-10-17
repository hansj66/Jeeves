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

#ifndef BUILD_H
#define BUILD_H
#include <QString>
#include <QStringList>
#include <QTime>
#include <QDomNode>
#include "FileDownloader.h"

class Build
{
public:
    typedef enum {
        Undefined,
        Windows,
        Mac,
        Linux
    } TARGET_OS;

    typedef enum {
        SUCCESS,
        FAILURE,
        BUILDING,
        ABORTED,
        UNKNOWN
    } STATUS;


    Build();
    Build(QDomNode node);
    QString ToDisplayString() const;

    QString Description() const { return m_description; }
    QString Name() const  { return m_name; }
    QString Url() const { return m_url; }
    bool Failed() const { return m_status == FAILURE; }
    STATUS Status() const { return m_status; }
    void Timestamp(QString timestamp) {m_timestamp = timestamp; }

    QString Name() { return m_name.replace("%20", " ");}

    void AddExcuse(QString excuse) { m_excuses.append(excuse); }

    QDateTime LastHeardFrom() const { return m_lastHeardFrom; }

    bool IsBuildable() const { return m_isBuildable; }

    TARGET_OS Target() const { return m_target;}

    bool IsConsistent() const;
    QString MachineShortName() const;

    bool parseXml(QString xmlString);
private:

    void setBuildable(bool isBuildable)  { m_isBuildable = isBuildable; }
    void setCulprits(QStringList culprits) {m_culprits = culprits; }
    void setDescription(QString description);
    void setLastHeardFrom(QDateTime time) { m_lastHeardFrom = time; }
    void setName(QString name) { m_name=name; }
    void setNumber(QString number) { m_number = number; }
    void setUrl(QString url) { m_url=url + "api/xml?depth=1"; }
    void setResult(QString result);
    void setStatus(STATUS status) { m_status = status;}

    QString m_name;
    QString m_url;
    QString m_number;
    STATUS m_status;
    QString m_timestamp;
    bool m_isBuildable;
    QStringList m_culprits;
    QStringList m_excuses;
    QDateTime m_lastHeardFrom;
    TARGET_OS m_target;
    QString m_description;
};

#endif // BUILD_H
