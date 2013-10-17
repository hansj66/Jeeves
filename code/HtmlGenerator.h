#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include <QObject>

#include "log.h"
#include "statusscreen.h"
#include "locator.h"
#include "builders.h"

class HtmlGenerator : public QObject
{
Q_OBJECT
public:
    explicit HtmlGenerator(Builders * builders, QObject *parent);

private:

signals:

private slots:
    void RefreshHtml();

private:
    Builders * m_builders;
    QTimer * m_refreshTimer;

};

#endif // HTMLGENERATOR_H
