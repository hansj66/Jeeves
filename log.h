#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>

class Log
{
private:
    static Log * m_pInstance;
    QString m_logFile;
    QString m_Messages;
    Log();
    QString TimeStamp() const;


public:
    static Log * Instance();
    ~Log();

    void LogToFile(QString logFile);
    QString LogFile() const;
    void Status(QString message, QString qualifier = QString()) const;
    void Error(QString message) const;
};

#endif // OPTIONS_H
