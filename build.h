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
#include <QStringList>

class Build
{
public:
    Build() {}
    Build(QString name, QString url, QString color);
    QString ToString() const;
    QString ToDisplayString() const;

    QString Name() const  { return m_name; }
    QString Url() const { return m_url; }
    QString Color() const { return m_color; }

    void Number(QString number) { m_number = number; }
    void Result(QString result) { m_result = result; }
    QString Result() const { return m_result; }
    bool Failed() const { return m_result == "FAILURE"; }
    bool Success() const { return m_result == "SUCCESS"; }
    void Timestamp(QString timestamp) {m_timestamp = timestamp; }
    void Culprits(QStringList culprits) {m_culprits = culprits; }
    void Name(QString name) { m_name=name; }
    void Url(QString url) { m_url=url; }
    void Color(QString color) { m_color=color; }
    void AddExcuse(QString excuse) {m_excuses.append(excuse); }

    bool IsConsistent() const;
    bool IsBuilding() const;

 private:
    QString m_name;
    QString m_url;
    QString m_color;
    QString m_number;
    QString m_result;
    QString m_timestamp;
    QStringList m_culprits;
    QStringList m_excuses;
};

#endif // BUILD_H
