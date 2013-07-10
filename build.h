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

#ifndef BUILD_H
#define BUILD_H

#include <QString>

class Build
{
public:
    Build() {}
    Build(QString name, QString url, QString color);
    void Name(QString name) { m_name=name; }
    void Url(QString url) { m_url=url; }
    void Color(QString color) { m_color=color; }
    QString Name() const  { return m_name; }
    QString Url() const { return m_url; }
    QString Color() const { return m_color; }

    QString ToString() const;

private:
    QString m_name;
    QString m_url;
    QString m_color;
};

#endif // BUILD_H
