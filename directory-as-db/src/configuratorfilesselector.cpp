#include "configuratorfilesselector.h"
#include <QDateTime>
#include <QTimer>
#include "matildalimits.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

ConfiguratorFilesSelector::ConfiguratorFilesSelector(QObject *parent) : QObject(parent)
{
    stopAll = false;
    useDefaultMethod2processJSON = true;
}

void ConfiguratorFilesSelector::setSelectData(QVariant v, QString datemask)
{
    selthisdata = v;
    dateMask = datemask;
}

void ConfiguratorFilesSelector::createDataProcessingObject()
{
    qDebug() << " ConfiguratorFilesSelector::createDataProcessingObject() ";
}

bool ConfiguratorFilesSelector::processThisJSON(const QJsonObject &j)
{
    return defaultMethod2processJSON(j, "?");
}

bool ConfiguratorFilesSelector::defaultMethod2processJSON(const QJsonObject &j, const QString &devid)
{
    if(j.isEmpty())
        return false;

    const QStringList lk = j.keys();
    QHash<QString,QString> oneitem;

    for(int i = 0, imax = lk.size(); i < imax; i++)
        oneitem.insert(lk.at(i), j.value(lk.at(i)).toString());

    MyListHashString data;
    data.append(oneitem);
    emit appendMeterData(devid, "", data);
    return false;

}

void ConfiguratorFilesSelector::startSelection()
{

    createDataProcessingObject();

    emit searchStarted();
    if(selectData() < 1)
        emit appendMeterData("", "", MyListHashString());
    emit searchFinished();

    deleteLater();
}

void ConfiguratorFilesSelector::setNewSyncDir(QString path)
{
    if(!path2syncDir.isEmpty())
        return;

    path2syncDir = path;
    QTimer::singleShot(222, this, SLOT(startSelection()));
}

void ConfiguratorFilesSelector::stopAllSlotDirect()
{
    stopAll = true;
}

qint64 ConfiguratorFilesSelector::selectData()
{

    qint64 counter = 0;
    const QVariantHash h = selthisdata.toHash();


    const QDateTime currdt = QDateTime::currentDateTimeUtc();
    const QDateTime todt = h.value("ToDT").toDateTime();
    const QDateTime fromdt = h.value("FromDT", currdt).toDateTime();

    const QString code = QString::number(h.value("code").toInt());

    const QDateTime maxdt = todt.isValid() ? todt : currdt;

    const QStringList onlyTheseNis = h.value("nil").toStringList();


    if(path2syncDir.isEmpty() || !fromdt.isValid())
        return counter;


    int month = fromdt.date().month();
    for(int j = 0, i = fromdt.date().year(), imax = maxdt.date().year(); j < MAX_DATABASE_SIZE && i <= imax; i++){
        const QString yearstr = QString::number(i).rightJustified(4, '0');

        if(stopAll)
            return counter;

        for( ; month < 13; month++){
            const QString monthstr = QString::number(month).rightJustified(2, '0');


            if(stopAll)
                return counter;

            const QString path2dir = QString("%1/%2/%3").arg(path2syncDir).arg(yearstr).arg(monthstr);
            QDir dir(path2dir);
            if(!dir.exists())
                continue;

            const QFileInfoList l = dir.entryInfoList(QString("%1_*").arg(code).split(" "), QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks, QDir::Name);



            for(int i = 0, imax = l.size(); i < imax; i++){
                const QFileInfo fi = l.at(i);

                const QStringList filenamel = fi.fileName().split("_");
                if(filenamel.size() < 4)
                    continue;

                const QDateTime created = QDateTime::fromMSecsSinceEpoch(filenamel.at(1).toLongLong());
                if(created < fromdt || created > maxdt)
                    continue;


                if(stopAll)
                    return counter;

                const QString devid = filenamel.at(3);
                if(devid.isEmpty())
                    continue;
                if(!onlyTheseNis.isEmpty() && !onlyTheseNis.contains(devid))
                    continue;

                QFile file(fi.absoluteFilePath());
                if(!file.open(QFile::ReadOnly))
                    continue;


                const QByteArray arr = file.read(MAX_PACKET_LEN);
                file.close();


                if( (useDefaultMethod2processJSON && defaultMethod2processJSON(QJsonDocument::fromJson(arr).object(), devid))
                        || (!useDefaultMethod2processJSON && processThisJSON(QJsonDocument::fromJson(arr).object())) )
                    counter++;



            }


        }
        month = 0;
    }

    return counter;
}
