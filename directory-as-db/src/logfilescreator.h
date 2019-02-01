#ifndef LOGFILESCREATOR_H
#define LOGFILESCREATOR_H

#include <QObject>

class LogFilesCreator : public QObject
{
    Q_OBJECT
public:
    explicit LogFilesCreator(QObject *parent = nullptr);

    QString getPath2syncDir() const;

    bool save2file(const qint64 &msec, const quint8 &pollcode, const quint8 &devtype, const QString &devid, const QByteArray &jsonline, QString &errstr);

    bool makeDirPath(const QString &path2dir);

    QString getFullPath2dir(const qint64 &msec);

signals:
    void giveMeAsyncDir();


public slots:

    void setNewSyncDir(QString path);


private:
    QString path2syncdir;

};

#endif // LOGFILESCREATOR_H
