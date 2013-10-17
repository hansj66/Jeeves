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


    void Append(Build build);
    void Append(QList<Build> builds);
    bool AnyFailed() const;
    Build At(int index) const;
    int Count() const;
    QStringList WaitMessages();
//    void UpdateLastHeardFrom(QString key, QDateTime time);
//    void UpdateName(QString key, QString name);
//    void UpdateUrl(QString key, QString url);
//    void UpdateNumber(QString key, QString number);
//    void UpdateBuildable(QString key, QString buildable);
//    void UpdateCulprits(QString key, QStringList culprits);
//    void UpdateBuilding(QString key, QString building);
//    void UpdateDescription(QString key, QString description);
//    int RemoveStale();
//    QString ToString();


//    int Count();
//    int Excluded();

//    Builds Filtered();
    bool Failed();

//    bool Failed(int index);
//    bool Success(int index);
//    bool IsBuilding(int index);

private:
    QList< Build  >  m_builds;
    //void Append( Build b);
    //bool IsFiltered(Build b);
};

#endif // BUILDS_H
