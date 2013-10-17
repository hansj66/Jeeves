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

StatusScreen::StatusScreen(Builders *  builders, QWidget *parent) :
    QMainWindow(parent),
    m_builders(builders),
    m_discoveredBuilds(0)
{
    m_started = QDateTime::currentDateTime();

    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    m_refreshTimer->start(4000);


    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    setWindowState(windowState() | Qt::WindowFullScreen);


    show();
    Refresh();
}

void StatusScreen::Refresh()
{
    RefreshLayout();
    RefreshData();
}


void StatusScreen::RefreshLayout()
{
    Builds builds = m_builders->builds();
    int nCount = builds.count();
    if (nCount == 0)
        return;


    if (m_discoveredBuilds != nCount)
    {
        m_discoveredBuilds = nCount;
        m_DisplayLines.clear();
        m_Icons.clear();

        m_lineHeight = (height()-14* nCount) / nCount;

        QVBoxLayout * layout = new QVBoxLayout();
        for (int i=0; i<nCount; i++)
        {
            QHBoxLayout * line = new QHBoxLayout();
            QPixmap image;
            Build::TARGET_OS os = builds.at(i)->Target();
            switch (os)
            {
                case Build::Windows: image = QPixmap(":/resources/windows-logo.png"); break;
                case Build::Mac: image = QPixmap(":/resources/osx_logo.jpg"); break;
                case Build::Linux: image = QPixmap(":/resources/linux-logo.jpg"); break;
                default: image = QPixmap(":/resources/undefined.png");
            }

            QLabel * label = new QLabel(this);
            QPixmap scaled = image.scaledToHeight(m_lineHeight);

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
    UpdateStyleSheets(m_builders->builds().count());
    for (int i=0; i<m_builders->builds().count(); i++)
    {
        QString message = QString("Waiting for response from : %1").arg(m_builders->builds().at(i)->Url());
        m_DisplayLines.at(i)->setText(message);
    }
}


void StatusScreen::UpdateStyleSheets(int nCount)
{
    if (0 == nCount)
        return;

    m_lineHeight = (height()-14* nCount) / nCount;

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

    Builds builds = m_builders->builds();

    if(builds.count() == 0)
        return;

    UpdateStyleSheets(builds.count());

    bool anyFailed = false;
    for(int i = 0; i < builds.count();++i)
    {
        Build * build = builds.at(i);
        if(build->Failed())
        {
            anyFailed = true;
            continue;
        }
    }
    if (anyFailed)
        m_mainWindow->setStyleSheet(QString("QWidget {background: black;}"));
    else
        m_mainWindow->setStyleSheet(QString("QWidget {background: white;}"));


    for(int i = 0; i < builds.count();++i)
    {
        Build * build = builds.at(i);
        if (build->IsBuilding())
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; color: black; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFCC, stop: 1 #FFFF00); font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));

        else if (build->Failed())
            m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF5555, stop: 1 #FF0000); font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));

        else if (build->Success())
              m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 1 #0000FF);; color: white; font-size: 12pt; font-weight:bold;}").arg(m_lineHeight));

        m_DisplayLines.at(i)->setText(build->ToDisplayString());
    }


    m_builders->RemoveStale();
 }
