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
    void RefreshHtml() const;

private:
    Builders * m_builders;

};

#endif // HTMLGENERATOR_H
