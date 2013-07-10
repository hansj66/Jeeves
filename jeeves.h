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

#ifndef JEEVES_H
#define JEEVES_H

#include <QCoreApplication>
#include <QtNetwork>
#include <QTimer>

#include "locator.h"
#include "interrogator.h"

class Jeeves : public QCoreApplication
{
    Q_OBJECT

public:
    Jeeves(QHostAddress broadcastAddress, int &argc, char **argv);

public slots:
    void OnJenkinsInstanceRefresh();

private:
    Locator * m_locator;
    Interrogator * m_interrogator;
    QTimer * m_broadcastTimer;
};

#endif // JEEVES_H
