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
#include "statusscreen.h"
#include <QLineEdit>
#include <QVBoxLayout>

StatusScreen::StatusScreen(QHostAddress broadcastAddress, QWidget *parent) :
    QMainWindow(parent),
    m_discoveredBuilds(0),
    m_refreshInterval(0)
{
    InitBroadcast(broadcastAddress);
    this->show();
}


void StatusScreen::InitBroadcast(QHostAddress broadcastAddress)
{
    m_locator = new Locator(broadcastAddress, this);
    m_interrogator = new Interrogator(&m_builds);
    connect(m_locator, SIGNAL(finished()), this, SLOT(OnJenkinsInstanceRefresh()));
    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, SIGNAL(timeout()), m_locator, SLOT(run()));
    m_broadcastTimer->start(5000);
}

void StatusScreen::OnJenkinsInstanceRefresh()
{
    QStringList apiList = m_locator->BuildMachineAPIs();
    Log::Instance()->Status(QString("Known build machines: %1").arg(apiList.count()));

    m_interrogator->Request(apiList);

    if (0 == m_refreshInterval)
    {
        // The implementation of the Interrogator class may be "wrong", but this seems to be the easiest way to avoid
        // memory leaking.
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
        delete m_interrogator;
        m_interrogator = new Interrogator(&m_builds);
    }

    Log::Instance()->Status(QString("Known builds: %1").arg(m_builds.Count()));
    Log::Instance()->Status(QString("Excluded builds: %1").arg(m_builds.Excluded()));

    if (m_discoveredBuilds != m_builds.Filtered().Count())
    {
        RefreshLayout();
        m_discoveredBuilds = m_builds.Filtered().Count();
        InitDisplayMessage();
    }

    RefreshData();
 }

void StatusScreen::RefreshLayout()
{
    if (m_builds.Filtered().Count() == 0)
        return;

    m_DisplayLines.clear();
    QVBoxLayout * layout = new QVBoxLayout();
    for (int i=0; i<m_builds.Filtered().Count(); i++)
    {
        QLineEdit * pEdit = new QLineEdit(this);
        pEdit->setReadOnly(true);
        m_DisplayLines.push_back(pEdit);
        layout->addWidget(pEdit);
    }

    QWidget * window = new QWidget(this);
    window->setStyleSheet("QWidget {  background: black;}");
    window->setLayout(layout);
    setCentralWidget(window);

    showMaximized();

    m_lineHeight = (height()-100) / m_DisplayLines.size();
 }

void StatusScreen::InitDisplayMessage()
{
    QStringList messages = m_builds.Filtered().WaitMessages();
    for (int i=0; i<messages.count(); i++)
    {
        m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF); font-size: 24pt; font-weight:bold;}").arg(m_lineHeight));
        m_DisplayLines.at(i)->setText(messages.at(i));
    }
}

void StatusScreen::RefreshData()
{
    m_refreshInterval ++;
    if (m_refreshInterval < 5)
        return;
    m_refreshInterval = 0;

    for (int i=0; i<m_builds.Filtered().Count(); i++)
    {
        if (m_builds.Filtered().Failed(i))
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF5555, stop: 1 #FF0000); font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));
        else if (m_builds.Filtered().Success(i))
              m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 1 #0000FF);; color: white; font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));
        else if (m_builds.Filtered().IsBuilding(i))
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF);; color: white; font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));

        m_DisplayLines.at(i)->setText(m_builds.Filtered().StatusMessage(i));
        //m_DisplayLines.at(i)->setText(QString("INDEX:%1").arg(i));
    }

    m_builds.RemoveStale();
 }


