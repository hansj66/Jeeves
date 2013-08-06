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
    m_interrogator = new Interrogator(this);
    connect(m_locator, SIGNAL(finished()), this, SLOT(OnJenkinsInstanceRefresh()));
    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, SIGNAL(timeout()), m_locator, SLOT(run()));
    m_broadcastTimer->start(1000);
}

void StatusScreen::OnJenkinsInstanceRefresh()
{
    QStringList apiList = m_locator->BuildMachineAPIs();
    Log::Instance()->Status(QString("Known build machines: %1").arg(apiList.count()));
    m_interrogator->Request(apiList);

    m_builds = m_interrogator->GetBuilds();
    Log::Instance()->Status(QString("Known builds: %1").arg(m_builds.count()));
    for (int i=0; i<m_builds.count(); i++)
        Log::Instance()->Status(m_builds.at(i).ToString());

    if (m_discoveredBuilds != m_builds.count())
    {
        RefreshLayout();
        m_discoveredBuilds = m_builds.count();
        InitDisplayMessage();
    }

    RefreshData();
}

void StatusScreen::RefreshLayout()
{
    m_DisplayLines.clear();
    QVBoxLayout * layout = new QVBoxLayout(this);
    for (int i=0; i<m_builds.count(); i++)
    {
        QLineEdit * pEdit = new QLineEdit(this);
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
    for (int i=0; i<m_builds.count(); i++)
    {
        m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF); font-size: 36pt; font-weight:bold;}").arg(m_lineHeight));
        m_DisplayLines.at(i)->setText(QString("Waiting for response from : %1").arg(m_builds.at(i).Url()));
    }
}

void StatusScreen::RefreshData()
{
    m_refreshInterval ++;
    if (m_refreshInterval < 5)
        return;
    m_refreshInterval = 0;

    for (int i=0; i<m_builds.count(); i++)
    {
        if (m_builds.at(i).IsConsistent())
        {
            if (m_builds.at(i).Failed())
                m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1px; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF5555, stop: 1 #FF0000); font-size: 36pt; font-weight:bold;}").arg(m_lineHeight));
            else if (m_builds.at(i).Success())
                  m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #5555FF, stop: 1 #0000FF);; color: white; font-size: 36pt; font-weight:bold;}").arg(m_lineHeight));
            else if (m_builds.at(i).IsBuilding())
                m_DisplayLines.at(i)->setStyleSheet(QString("QLineEdit {  height: %1; border: 2px solid gray; border-radius: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #FFFFFF);; color: white; font-size: 36pt; font-weight:bold;}").arg(m_lineHeight));

            m_DisplayLines.at(i)->setText(m_builds.at(i).ToDisplayString());
        }
    }
 }

