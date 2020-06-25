#include "logfilescreator.h"

#include "src/shared/compressfilehelper.h"

#include <QDir>
#include <QDateTime>


LogFilesCreator::LogFilesCreator(QObject *parent) : QObject(parent)
{

}

QString LogFilesCreator::getPath2syncDir() const
{
    return path2syncdir;
}

bool LogFilesCreator::save2file(const qint64 &msec, const quint8 &pollcode, const quint8 &devtype, const QString &devid, const QByteArray &jsonline, QString &errstr)
{

    if(path2syncdir.isEmpty()){
        errstr = tr("The sync directory path is not set");
        return false;
    }

    const QString path2dir = getFullPath2dir(msec);
    if(path2dir.isEmpty()){
        errstr = tr("It is impossible to write to the sync directory");
        return false;
    }

    const QString filename = QString("%1/%2_%3_%4_%5").arg(path2dir).arg(int(pollcode)).arg(msec).arg(int(devtype)).arg(devid);

    return CompressFileHelper::writeArr2file(jsonline, filename, 11, errstr);
}



bool LogFilesCreator::makeDirPath(const QString &path2dir)
{
    QDir dir(path2dir);
    if(dir.exists())
        return true;

    return dir.mkpath(path2dir);
}



QString LogFilesCreator::getFullPath2dir(const qint64 &msec)
{
    const QString path = QString("%1/%2").arg(getPath2syncDir()).arg(QDateTime::fromMSecsSinceEpoch(msec).toString("yyyy/MM"));
    return makeDirPath(path) ? path : QString();
}

void LogFilesCreator::setNewSyncDir(QString path)
{
    path2syncdir = path;
    emit onSyncPathReceived();
}

