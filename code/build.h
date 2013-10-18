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
        Undefined = 0,
        Windows,
        Mac,
        Linux
    } TARGET_OS;

    typedef enum {
        SUCCESS = 0,
        FAILURE,
        BUILDING,
        ABORTED,
        UNKNOWN
    } STATUS;


    Build();
    Build(QDomNode node);


    QString       Description()      const { return m_description; }
    bool          Failed()           const { return m_status == FAILURE; }
    bool          IsBuildable()      const { return m_isBuildable; }
    bool          IsConsistent()     const;
    QString       LastBuildUrl()     const  { return m_lastBuildUrl; }
    QDateTime     LastHeardFrom()    const { return m_lastHeardFrom; }
    QString       MachineShortName() const;
    QString       Name()             const  { return m_name; }
    bool          parseLastBuildXml(const QByteArray & xmlString);
    bool          parseXml(const QByteArray & xmlString);
    STATUS        Status()           const { return m_status; }
    TARGET_OS     Target()           const { return m_target;}
    QString       ToDisplayString()  const;
    QString       Url()              const { return m_url; }

private:
    void setBuildable(const bool & isBuildable)      { m_isBuildable = isBuildable; }
    void setCulprits(const QStringList & culprits)   {m_culprits = culprits; }
    void setDescription(const QString & description);
    void setLastBuildUrl(const QString &lastBuildUrl){m_lastBuildUrl = lastBuildUrl + "api/xml";}
    void setLastHeardFrom(const QDateTime & time)    { m_lastHeardFrom = time; }
    void setName(const QString & name)               { m_name=name; }
    void setNumber(const QString & number)           { m_number = number; }
    void setUrl(const QString & url)                 { m_url=url + "api/xml"; }
    void setResult(const QString & result);
    void setStatus(STATUS status)                    { m_status = status;}

    QStringList m_culprits;
    QString m_description;
    bool    m_isBuildable;
    QDateTime m_lastHeardFrom;
    QString m_name;
    QString m_number;
    STATUS  m_status;
    TARGET_OS m_target;
    QString m_timestamp;
    QString m_url;
    QString m_lastBuildUrl;




};

#endif // BUILD_H
