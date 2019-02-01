#ifndef CONFIGURATORFILESCREATOR_H
#define CONFIGURATORFILESCREATOR_H

#include "logfilescreator.h"

class ConfiguratorFilesCreator : public LogFilesCreator
{
    Q_OBJECT
public:
    explicit ConfiguratorFilesCreator(QObject *parent = nullptr);

signals:

    void showMess(QString mess);

    void append2applog(QString line);

public slots:
    void saveThisData2database(qint64 msec, quint8 meterType, quint8 pollCode, QString idstr, QStringList lkeys, QStringList ldata);

};

#endif // CONFIGURATORFILESCREATOR_H
