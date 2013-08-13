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

#include "build.h"

Build::Build() :
    m_lastHeardFrom(QDateTime::currentDateTime())
{

}


QString Build::ToString() const
{
    QString build = QString("\n\n\tname=%1\n\tlast build number=%2\n\turl=%3\n\tResult=%4\n\tAlive=%5\n\tIs building=%6\n\tIs buildable=%7\n").arg(Name()).arg(m_number).arg(m_url).arg(m_result).arg(m_lastHeardFrom.toString()).arg(m_isBuilding).arg(m_isBuildable);

    if (m_culprits.length() == 0)
        return build;

    build.append("\tculprits=");
    for (int i=0; i<m_culprits.length(); i++)
        build.append(QString("%1,").arg(m_culprits.at(i)));
    return build.left(build.length()-1);
}

QString Build::ToDisplayString() const
{
    QString build = QString("%1:%2 (Build #%3) ").arg(MachineShortName()).arg(Name()).arg(m_number);

    if (m_culprits.length() == 0)
        return build;

    build.append("Culprit");
    if (m_culprits.length() > 1)
        build.append("s");

    build.append(" : ");
    for (int i=0; i<m_culprits.length(); i++)
        build.append(QString("%1,").arg(m_culprits.at(i)));
    return build.left(build.length()-1);
}

QString Build::MachineShortName() const
{
    return m_url.right(m_url.length()-7).split("/").at(0).split(":").at(0);
}

bool Build::IsConsistent() const
{
    if ((m_name.isEmpty()) || m_url.isEmpty() || m_number.isEmpty())
            return false;
    return true;
}



