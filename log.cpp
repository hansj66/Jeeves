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















