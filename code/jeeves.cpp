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
#include "statusscreen.h"

Jeeves::Jeeves(QHostAddress broadcastAddress, int &argc, char **argv) :
    QApplication(argc, argv)
{


    m_locater = new Locator(broadcastAddress, this);
    m_builders = new Builders();

    connect(m_locater, SIGNAL(buildersDisapeared(QStringList)),m_builders,SLOT(OnRemoveBuilders(QStringList)));
    connect(m_locater, SIGNAL(buildersFound(QStringList)),m_builders,SLOT(OnAddBuilders(QStringList)));

    new StatusScreen(m_builders);


 }

