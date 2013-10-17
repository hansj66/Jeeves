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
    m_refreshTimer->start(3000);
}

void HtmlGenerator::RefreshHtml()
{
    QFile outfile("./index.html");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

    Builds builds = m_builders->builds();
    if(builds.count() == 0) return;

    QTextStream out(&outfile);
    out << "<!DOCTYPE html>\n";
    out << "<html lang=\"en-US\">\n";


    out << "<HEAD>\n";
    out << "<title>Jeeves is awesome</title>\n";
    out << "<meta http-equiv=\"refresh\" content=\"2\" >\n";
    out << "<link rel=\"stylesheet\" type=\"text/css\" href=\"jeeves.css\">";

    out << "</HEAD>\n";
    out << "<BODY>\n";


    out << "<TABLE>\n";

    for(int i = 0; i < builds.count();++i)
    {
        Build * build = builds.at(i);


        QString color;

        Build::STATUS status = build->Status();
        switch (status)
        {
        case Build::BUILDING:
            color = "yellow";
            break;
        case Build::FAILURE:
            color = "red";
            break;
        case Build::SUCCESS:
            color = "66CCFF";
            break;
        case Build::ABORTED:
            color = "grey";
            break;
        default:
            color = "white";
            break;
        }

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

        out << "<TR>" << QString("<TD bgcolor=\"white\">") << QString("<img src=\"%1\"  width=\"75\" height=\"75\" alt=\"build machine\"  >").arg(imageFile)  << "</TD>";
        out << QString("<TD bgcolor=\"%1\">").arg(color) << "<H1>" << build->ToDisplayString() << "</H1></TD>";
        out << "</TR>\n";
    }

    out << "</TABLE>\n</BODY>\n</html>\n";

    Log::Instance()->Status(QDateTime::currentDateTime().toString(), QString("Time: "));
    outfile.close();
}


