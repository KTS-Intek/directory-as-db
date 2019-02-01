#ifndef CONFIGURATORFILESSELECTOR_H
#define CONFIGURATORFILESSELECTOR_H

#include <QObject>
#include <QVariantHash>
#include "classmanagertypes.h"

class ConfiguratorFilesSelector : public QObject
{
    Q_OBJECT
public:
    explicit ConfiguratorFilesSelector(QObject *parent = nullptr);

    QVariant selthisdata;
    QString path2syncDir;
    bool stopAll;
    QString dateMask;

    void setSelectData(QVariant v, QString datemask);


    virtual void createDataProcessingObject();//to a child


signals:
    void appendData2model(QVariantHash h);

    void appendMeterData(QString ni, QString sn, MyListHashString data);



//    void giveMeAsyncDir();

public slots:
    void startSelection();

    void setNewSyncDir(QString path);

    void stopAllSlotDirect();

private:
    qint64 selectData();
};

#endif // CONFIGURATORFILESSELECTOR_H
