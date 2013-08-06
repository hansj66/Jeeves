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

#include <QTimer>
#include <QtNetwork>
#include <iostream>

#include "errors.h"
#include "log.h"
#include "jeeves.h"

using namespace std;

void Usage()
{
    cout << "Usage: Jeeves [log=<logfile>] address=<broadcast address>\n";
    cout << "   The broadcast address of a subnet is always the highest address in the subnet.";
    cout << "   Example: If your subnet mask is 255.255.255.0 and your IP is 192.168.146.1, then the broadcast address is 192.168.146.255.";
}

int Error(int errorCode, QString bailoutMessage)
{
    Log::Instance()->Error(bailoutMessage);
    Usage();
    return errorCode;
}


int main(int argc, char *argv[])
{
    const QString logParam = "log=";
    const QString addressParam = "address=";

    QString addressArg;

    for (int i=1; i<argc; i++)
    {
        QString a = QString::fromLocal8Bit(argv[i]);

        if (logParam == a.left(logParam.length()).toLower())
        {
            Log::Instance()->LogToFile(a.mid(logParam.length()));
        }
        else if (addressParam == a.left(addressParam.length()).toLower())
        {
            addressArg = a.mid(addressParam.length());
        }
        else
        {
           return Error(UNRECOGNIZED_ARGUMENT, QString("Unrecognized command line argument: %1").arg(argv[i]));
        }
    }

    QHostAddress broadcastAddress = QHostAddress(addressArg);
    if (addressArg.isEmpty() || (broadcastAddress.isNull()))
        return Error(BAD_BROADCAST_ADDRESS, "Bad or missing broadcast address !");

    Log::Instance()->Status(QString("Broadcast address: %1").arg(addressArg));

    std::cout << "\nHi ! I am Jeeves. Hang on...\n";

    Jeeves jeeves(broadcastAddress, argc, argv);

    return jeeves.exec();
}
