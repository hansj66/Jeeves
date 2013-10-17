#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    void Get(QString url);
    virtual ~FileDownloader();

    QByteArray downloadedData(QString m_url) const;

signals:
        void downloaded(QString url);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QMap<QString, QByteArray> m_DownloadedData;

};

#endif // FILEDOWNLOADER_H
