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
