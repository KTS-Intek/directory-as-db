#include "logswriter.h"
#include <QTextStream>
#include <QDateTime>

LogsWriter::LogsWriter(const QString &devicename, QObject *parent) : ConfiguratorFilesCreator(parent), devicename(devicename)
{
    logswriterobject = 0;
    allow2usecache = true;

}

void LogsWriter::onThreadStarted()
{
    if(devicename.isEmpty()){
        kickOffAndDie();
        return;
    }

    connect(this, &LogsWriter::onSyncPathReceived, this, &LogsWriter::preparyTheFile);


    emit giveMeAsyncDir();


}

void LogsWriter::preparyTheFile()
{
    if(logswriterobject)
        return;

    disconnect(this, &LogsWriter::onSyncPathReceived, this, &LogsWriter::preparyTheFile);

    setNewSyncDir(QString("%1/logs").arg(getPath2syncDir()));


    if(!createAfileObject()){
        kickOffAndDie();
        return;
    }

    if(allow2usecache){
        allow2usecache = false;
        for(int i = 0, imax = cache.size(); i < imax; i++)
            appendTheseLines(cache.at(i));
        cache.clear();
    }


}

void LogsWriter::appendTheseLines(QString lines)
{
    if(logswriterobject){

        bool r = logswriterobject->isOpen();
        if(!r){
            r = logswriterobject->open(QFile::WriteOnly|QFile::Append);
        }

        if(r){
            QTextStream stream(logswriterobject);
            stream << lines << endl;

        }
         return;
    }
    if(allow2usecache)
        cache.append(lines);

}

void LogsWriter::kickOffAndDie()
{
    if(logswriterobject && logswriterobject->isOpen())
        logswriterobject->close();
    deleteLater();
}

bool LogsWriter::createAfileObject()
{

    logswriterobject = new QFile(this);


    if(getPath2syncDir().isEmpty()){
//        errstr = tr("The sync directory path is not set");
        return false;
    }
    const qint64 msec = QDateTime::currentMSecsSinceEpoch();

    const QString path2dir = getFullPath2dir(msec);
    if(path2dir.isEmpty()){
//        errstr = tr("It is impossible to write to the sync directory");
        return false;
    }

    const QString filename = QString("%1/%2_%3_%4_%5").arg(path2dir).arg(int(0xFF)).arg(msec).arg(int(0xFF)).arg(devicename);

    logswriterobject->setFileName(filename);

    if(logswriterobject->open(QFile::WriteOnly)){
        return true;
    }
    return false;


}
