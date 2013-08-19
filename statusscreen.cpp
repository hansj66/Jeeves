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
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>

StatusScreen::StatusScreen(QHostAddress broadcastAddress, QWidget *parent) :
    QMainWindow(parent),
    m_discoveredBuilds(0),
    m_refreshInterval(0)
{
    m_started = QDateTime::currentDateTime();
    InitBroadcast(broadcastAddress);
    show();
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
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3000);
        delete m_interrogator;
        m_interrogator = new Interrogator(&m_builds);
    }

    Log::Instance()->Status(QString("Known builds: %1").arg(m_builds.Count()));
    Log::Instance()->Status(QString("Excluded builds: %1").arg(m_builds.Excluded()));

    int nCount = m_builds.Filtered().Count();

    RefreshLayout(nCount);
    RefreshData();
 }

void StatusScreen::RefreshLayout(int nCount)
{
    if (nCount == 0)
        return;

    showMaximized();

    if (m_discoveredBuilds != nCount)
    {
        m_discoveredBuilds = nCount;
        m_DisplayLines.clear();
        m_Icons.clear();

        m_lineHeight = (height()-100) / nCount;

        QVBoxLayout * layout = new QVBoxLayout();
        for (int i=0; i<nCount; i++)
        {
            QHBoxLayout * line = new QHBoxLayout();
            QPixmap * image;
            Build::TARGET_OS os = m_builds.Filtered().Target(i);
            switch (os)
            {
                case Build::Windows: image = new QPixmap("windows-logo.png"); break;
                case Build::Mac: image = new QPixmap("osx_logo.jpg"); break;
                case Build::Linux: image = new QPixmap("linux-logo.jpg"); break;
                default: image = new QPixmap("undefined.png");
            }

            QLabel * label = new QLabel(this);
            QPixmap scaled = image->scaledToHeight(m_lineHeight);
            label->setPixmap(scaled);
            label->resize(50, 50);
            line->addWidget(label);

            QLineEdit * pEdit = new QLineEdit(this);
            pEdit->setReadOnly(true);
            m_DisplayLines.push_back(pEdit);
            m_Icons.push_back(label);
            line->addWidget(pEdit);

            layout->addLayout(line);
        }
        m_mainWindow = new QWidget(this);
        m_mainWindow->setStyleSheet("QWidget {  background: white;}");
        m_mainWindow->setLayout(layout);
        setCentralWidget(m_mainWindow);

        InitDisplayMessage();
    }
 }

void StatusScreen::InitDisplayMessage()
{
    QStringList messages = m_builds.Filtered().WaitMessages();
    UpdateStyleSheets(messages.count());
    for (int i=0; i<messages.count(); i++)
    {
        m_DisplayLines.at(i)->setText(messages.at(i));
    }
}


void StatusScreen::UpdateStyleSheets(int nCount)
{
    if (0 == nCount)
        return;

    m_lineHeight = (height()-100) / nCount;

     for (int i=0; i<nCount; i++)
     {
         m_Icons.at(i)->setStyleSheet(QString("QLabel {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF); font-size: 24pt; font-weight:bold;}").arg(m_lineHeight));
         m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF); font-size: 24pt; font-weight:bold;}").arg(m_lineHeight));
     }
}

void StatusScreen::RefreshUpTime()
{
    setWindowTitle(QString("Jeevs has been up and running for %1 days.").arg(QString::number((double)(m_started.secsTo(QDateTime::currentDateTime()))/86400, 'f', 4)));
 }

void StatusScreen::RefreshData()
{
    RefreshUpTime();

    m_refreshInterval ++;
    if (m_refreshInterval < 5)
        return;
    m_refreshInterval = 0;

    Builds b = m_builds.Filtered();
    UpdateStyleSheets(b.Count());

    if (b.Failed())
        m_mainWindow->setStyleSheet(QString("QWidget {background: black;}"));
    else
       m_mainWindow->setStyleSheet(QString("QWidget {background: white;}"));

    for (int i=0; i<b.Count(); i++)
    {
        if (b.Failed(i))
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF5555, stop: 1 #FF0000); font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));
        else if (b.Success(i))
              m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 1 #0000FF);; color: white; font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));
        else if (b.IsBuilding(i))
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; color: black; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFCC, stop: 1 #FFFF00); font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));

        m_DisplayLines.at(i)->setText(b.StatusMessage(i));
    }

    m_builds.RemoveStale();
 }
