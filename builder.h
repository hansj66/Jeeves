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

#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>
#include <QString>

class Builder: QObject
{
    Q_OBJECT

public:
    Builder(QString xml, QObject * parent);
    QString API();

private:
    QString rootUrl(QString xml);
    QString tagData(QString tag);

    QString m_API;
    QString m_XML;
    QString m_Target;
};

#endif // BUILDER_H
