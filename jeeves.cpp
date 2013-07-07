#include "jeeves.h"
#include "log.h"

Jeeves::Jeeves(QHostAddress broadcastAddress, int &argc, char **argv) :
    QCoreApplication(argc, argv)
{
    m_locator = new Locator(broadcastAddress, this);
    connect(m_locator, SIGNAL(finished()), this, SLOT(OnJenkinsInstanceRefresh()));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), m_locator, SLOT(run()));
    m_timer->start(1000);
 }


void Jeeves::OnJenkinsInstanceRefresh()
{
    Log::Instance()->Status("Toodledoo !!!\n");
    //emit quit();
}
