#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{

}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::Get(QString url)
{
     connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                 this,SLOT(fileDownloaded(QNetworkReply*)));
     QNetworkRequest request(url);
     m_WebCtrl.get(request);

}


void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QUrl url = pReply->url();
    m_DownloadedData[url.toString()] = pReply->readAll();

    //emit a signal
    pReply->deleteLater();
    emit downloaded(url.toString());
}

QByteArray FileDownloader::downloadedData(QString url) const
{
    return m_DownloadedData[url];
}
