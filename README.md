Jeeves is a monitoring tool for discovery and aggregation of information from multiple Jenkins build machines.

Copyright 2013 Hans Jørgen Grimstad. (hansj66@gmail.com)

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

You should be able to compile Jeeves on any platform supported by the Qt framework.

Tested compilers (so far):

- Microsoft Visual C++ Compiler 10.0
- Clang-425.0.24 (Mac)
- gcc-4.2 (pretty old, but latest gcc on Mac)
- Debian Clang 3.0-6l+rpil (Raspberry)


Firewalls
----------
If your build machines are hiding behind a firewall, you should add a rule to allow incoming traffic on port 33848.

In Linux you can do this with by issuing the following commands

sudo iptables -A INPUT -p udp --dport 33848 -j ACCEPT
sudo service iptables save
reboot

Screen savers / Screen blanking
------------------------------------
You should disable any screen blanking, power save options.

On a Raspberry, running LXDE, you can do this by issuing the following commands:

xset s off
xset -dpms
