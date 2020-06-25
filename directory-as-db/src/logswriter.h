#ifndef LOGSWRITER_H
#define LOGSWRITER_H

#include "configuratorfilescreator.h"

#include <QFile>


class LogsWriter : public ConfiguratorFilesCreator
{
    Q_OBJECT
public:
    explicit LogsWriter(const QString &devicename, QObject *parent = nullptr);

    QString devicename;


    QFile *logswriterobject;


    bool allow2usecache;
    QStringList cache;


signals:


public slots:
    void onThreadStarted();

    void preparyTheFile();


    void appendTheseLines(QString lines);

    void kickOffAndDie();

private:
    bool createAfileObject();

};

#endif // LOGSWRITER_H
