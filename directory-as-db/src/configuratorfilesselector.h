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

    bool useDefaultMethod2processJSON;

    void setSelectData(QVariant v, QString datemask);


    virtual void createDataProcessingObject();//to a child

    virtual bool processThisJSON(const QJsonObject &j);

    bool defaultMethod2processJSON(const QJsonObject &j, const QString &devid);


signals:
    void appendData2model(QVariantHash h);

    void appendMeterData(QString ni, QString sn, MyListHashString data);


    void searchStarted();

    void searchFinished();


//    void giveMeAsyncDir();

public slots:
    void startSelection();

    void setNewSyncDir(QString path);

    void stopAllSlotDirect();

private:
    qint64 selectData();
};

#endif // CONFIGURATORFILESSELECTOR_H
