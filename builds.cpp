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

#include "builds.h"

#include <QMapIterator>
#include <QDebug>

Builds::Builds()
{
}

void Builds::UpdateLastHeardFrom(QString key, QDateTime time)
{
    m_builds[key].LastHeardFrom(time);
}

void Builds::UpdateName(QString key, QString name)
{
      m_builds[key].Name(name);
}

void Builds::UpdateUrl(QString key, QString url)
{
    m_builds[key].Url(url);
}

void Builds::UpdateNumber(QString key, QString number)
{
    m_builds[key].Number(number);
}

void Builds::UpdateBuildable(QString key, QString buildable)
{
    m_builds[key].Buildable(buildable);
}

void Builds::UpdateResult(QString key, QString result)
{
    m_builds[key].Result(result);
}

void Builds::UpdateCulprits(QString key, QStringList culprits)
{
    m_builds[key].Culprits(culprits);
}

void Builds::UpdateBuilding(QString key, QString building)
{
    m_builds[key].Building(building);
}

void Builds::RemoveStale()
{
    QMap<QString, Build>::Iterator it;

    for (it=m_builds.begin(); it != m_builds.end(); it++)
    {
        if (it.value().LastHeardFrom().addSecs(30) < QDateTime::currentDateTime())
        {
            it = m_builds.erase(it);
        }
    }
}

int Builds::Filtered()
{
    QMapIterator<QString, Build> i(m_builds);
    int count= 0;
    while (i.hasNext())
    {
        i.next();
        if (IsFiltered(i.value()))
            count++;
    }
    return count;
}

int Builds::Count()
{
    QMapIterator<QString, Build> i(m_builds);
    int count= 0;
    while (i.hasNext())
    {
        i.next();
        if (!IsFiltered(i.value()))
            count++;
    }
    return count;
}

bool Builds::IsFiltered(Build b)
{
    return (!b.IsBuildable());
}

QStringList Builds::WaitMessages()
{
    QStringList messages;

    QMapIterator<QString, Build> i(m_builds);
    while (i.hasNext())
    {
        i.next();
        if (!IsFiltered(i.value()))
            messages.append(QString("Waiting for response from : %1").arg(i.value().Url()));
    }
    return messages;
}


bool Builds::Failed(int index)
{
    QMapIterator<QString, Build> it(m_builds);
    int i= 0;
    while (it.hasNext())
    {
        it.next();
        if (i == index)
            return it.value().Failed();
        if (!IsFiltered(it.value()))
            i++;
    }
    return true;
}

bool Builds::Success(int index)
{
    QMapIterator<QString, Build> it(m_builds);
    int i= 0;
    while (it.hasNext())
    {
        it.next();
        if (i == index)
            return it.value().Success();
        if (!IsFiltered(it.value()))
            i++;
    }
    return false;
}

bool Builds::IsBuilding(int index)
{
    QMapIterator<QString, Build> it(m_builds);
    int i= 0;
    while (it.hasNext())
    {
        it.next();
        if (i == index)
            return it.value().IsBuilding();
        if (!IsFiltered(it.value()))
            i++;
    }
    return false;
}

QString Builds::StatusMessage(int index)
{
    QMapIterator<QString, Build> it(m_builds);
    int i= 0;
    while (it.hasNext())
    {
        it.next();
        if (i == index)
            return it.value().ToDisplayString();
        if (!IsFiltered(it.value()))
            i++;
    }
    return "";
}



