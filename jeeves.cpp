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

#include "jeeves.h"
#include "log.h"
#include "interrogator.h"

Jeeves::Jeeves(QHostAddress broadcastAddress, int &argc, char **argv) :
    QCoreApplication(argc, argv)
{
    m_locator = new Locator(broadcastAddress, this);
    m_interrogator = new Interrogator(this);
    connect(m_locator, SIGNAL(finished()), this, SLOT(OnJenkinsInstanceRefresh()));
    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, SIGNAL(timeout()), m_locator, SLOT(run()));
    m_broadcastTimer->start(1000);
 }


void Jeeves::OnJenkinsInstanceRefresh()
{
    QStringList apiList = m_locator->BuildMachineAPIs();
    Log::Instance()->Status(QString("Known build machines: %1").arg(apiList.count()));
    m_interrogator->Request(apiList);

    QList<Build> builds = m_interrogator->GetBuilds();
    Log::Instance()->Status(QString("Known builds: %1").arg(builds.count()));
    for (int i=0; i<builds.count(); i++)
        Log::Instance()->Status(builds.at(i).ToString());

    //emit quit();
}
