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
#include "HtmlGenerator.h"

Jeeves::Jeeves(QHostAddress broadcastAddress, Mode mode, int &argc, char **argv) :
    QApplication(argc, argv)
{
    m_locater = new Locator(broadcastAddress, this);
    m_builders = new Builders();

    connect(m_locater, SIGNAL(buildersDisapeared(QStringList)),m_builders,SLOT(OnRemoveBuilders(QStringList)));
    connect(m_locater, SIGNAL(buildersFound(QStringList)),m_builders,SLOT(OnAddBuilders(QStringList)));

    switch (mode)
    {
        case GUI:
            new StatusScreen(m_builders); break;
        case HTML:
           QPixmap image(":/resources/windows-logo.png");
           image.save("windows-logo.png");

           image.load(":/resources/osx_logo.jpg");
           image.save("osx_logo.jpg");

           image.load(":/resources/linux-logo.jpg");
           image.save("linux-logo.jpg");

           image.load(":/resources/undefined.png");
           image.save("undefined.png");
           new HtmlGenerator(m_builders,this);
           break;
    }


 }

