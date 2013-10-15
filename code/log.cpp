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

#include "log.h"

#include <QFile>
#include <QDateTime>
#include <iostream>

Log * Log::m_pInstance = NULL;

Log::Log()
{
    if (m_logFile.isEmpty())
        return;

    QFile log(m_logFile);
    log.remove();
}

Log::~Log()
{
    delete m_pInstance;
    m_pInstance = NULL;
}

void Log::LogToFile(QString logFile)
{
    m_logFile = logFile;
}

QString Log::LogFile() const
{
    return m_logFile;
}

Log * Log::Instance()
{
    if (!m_pInstance)
        m_pInstance = new Log();
    return m_pInstance;
}

QString Log::TimeStamp() const
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

void Log::Error(QString message) const
{
    Status(message, "ERROR: ");
}

void Log::Status(QString message, QString qualifier) const
{
    if (m_logFile.isEmpty())
    {
        std::cout << QString("%1 %2\n").arg(qualifier).arg(message).toStdString();
        return;
    }

    QFile log(m_logFile);
    log.open(QFile::Append);
    log.write(QString("%1: %2 %3\n").arg(TimeStamp()).arg(qualifier).arg(message).toStdString().c_str());
    log.close();
}















