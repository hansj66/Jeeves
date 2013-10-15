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

#ifndef BUILDS_H
#define BUILDS_H

#include <QString>
#include <QStringList>
#include <QMap>
#include "build.h"

class Builds
{
public:
    explicit Builds();
    void UpdateLastHeardFrom(QString key, QDateTime time);
    void UpdateName(QString key, QString name);
    void UpdateUrl(QString key, QString url);
    void UpdateNumber(QString key, QString number);
    void UpdateBuildable(QString key, QString buildable);
    void UpdateResult(QString key, QString result);
    void UpdateCulprits(QString key, QStringList culprits);
    void UpdateBuilding(QString key, QString building);
    void UpdateDescription(QString key, QString description);
    int RemoveStale();
    QString ToString();

    QStringList WaitMessages();

    int Count();
    int Excluded();

    Builds Filtered();
    bool Failed();

    bool Failed(int index);
    bool Success(int index);
    bool IsBuilding(int index);
    QString StatusMessage(int index);
    Build::TARGET_OS Target(int index);

private:
    QMap<QString, Build>  m_builds;
    void Append(QString key, Build b);
    bool IsFiltered(Build b);
};

#endif // BUILDS_H
