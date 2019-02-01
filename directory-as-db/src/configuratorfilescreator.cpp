#include "configuratorfilescreator.h"


#include <QJsonObject>
#include <QJsonDocument>

ConfiguratorFilesCreator::ConfiguratorFilesCreator(QObject *parent) : LogFilesCreator(parent)
{

}

void ConfiguratorFilesCreator::saveThisData2database(qint64 msec, quint8 meterType, quint8 pollCode, QString idstr, QStringList lkeys, QStringList ldata)
{

    QJsonObject j;

    for(int i = 0, imax = lkeys.size(); i < imax; i++)
        j.insert(lkeys.at(i), ldata.at(i));

    QString mess;
    if(!save2file(msec, pollCode, meterType, idstr, QJsonDocument(j).toJson(QJsonDocument::Compact), mess )){
        emit append2applog(tr("Can't save to the file '%1', error '%2'").arg(idstr).arg(mess));
    }

    if(!mess.isEmpty())
        emit showMess(mess);

}
