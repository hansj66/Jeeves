#include "FileDownloader.h"
#include "log.h"

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                this,SLOT(fileDownloaded(QNetworkReply*)));
}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::Get(QString url)
{
     QNetworkRequest request(url);
     m_WebCtrl.get(request);

}


void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QString url = pReply->url().toString();
    m_DownloadedData[url] = pReply->readAll();


    //emit a signal
    pReply->deleteLater();
    emit downloaded(url);
}

QByteArray FileDownloader::downloadedData(const QString & url)
{
    QByteArray downloaded = m_DownloadedData[url];
    m_DownloadedData.remove(url);
    return downloaded;
}
