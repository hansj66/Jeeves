#include <QtCore>
#include <QObject>
#include "HtmlGenerator.h"


HtmlGenerator::HtmlGenerator(Builders * builders, QObject * parent) :
    QObject(parent)
{
    m_builders = builders;

    RefreshHtml();

    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(RefreshHtml()));
    m_refreshTimer->start(4000);
}

void HtmlGenerator::RefreshHtml()
{
    QFile outfile("./Jeeves.html");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

    Builds builds = m_builders->builds();
    if(builds.count() == 0) return;

    QTextStream out(&outfile);
    out << "<HEAD>";
    out << "<title>Jeeves is awesome</title>";
    out << "<meta http-equiv=\"refresh\" content=\"2\" >";

    out << "</HEAD>\n";
    out << "<BODY>\n";


    out << "<TABLE border=\"1\"   width=\"100%\" height=\"100%\">";

    for(int i = 0; i < builds.count();++i)
    {
        Build * build = builds.at(i);


        QString color;
        if (build->IsBuilding())
           color = "yellow";
        else if (build->Failed())
           color = "red";
        else if (build->Success())
           color = "66CCFF";

        QString imageFile;
        Build::TARGET_OS os = builds.at(i)->Target();
        switch (os)
        {
            case Build::Windows:
                imageFile = "windows-logo.png"; break;
            case Build::Mac:
                imageFile = "osx_logo.jpg"; break;
            case Build::Linux:
                imageFile = "linux-logo.jpg"; break;
            default:
                imageFile = "undefined.png";
        }

        out << "<TR>" << QString("<TD width=\"75\" height=\"75\"  bgcolor=\"white\">") << QString("<img src=\"%1\"  width=\"75\" height=\"75\"  >").arg(imageFile)  << "</TD>";
        out << QString("<TD bgcolor=\"%1\" style=\"vertical-align:middle\">").arg(color) << "<b><font size=\"24\">  " << build->ToDisplayString() << "</font><b></TD>";
        out << "</TR>\n";
    }

    out << "</TABLE></BODY>\n";

    Log::Instance()->Status(QDateTime::currentDateTime().toString(), QString("Time: "));
    outfile.close();
}
