#ifndef SOPSTORECLINET_H
#define SOPSTORECLINET_H
#include "sopstoreclinet_global.h"
#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDBusInterface>
#include <QFile>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

#include <QObject>
#include <cconfig.h>
#include "consttype.h"

class  SopStoreClinet : public QObject
{
    Q_OBJECT
public:
    explicit SopStoreClinet(QObject *parent = 0);
    ~SopStoreClinet();

    //Q_INVOKABLE bool isInstallApp(QString userId,QString appId);

    Q_INVOKABLE void writeData(QString content);
    Q_INVOKABLE QString readData();

    Q_INVOKABLE void downloadApp(QString pkgName,QString url);
    Q_INVOKABLE void login(QString json);
    Q_INVOKABLE void preLogin(QString json);
    Q_INVOKABLE void loginout();

    Q_INVOKABLE void slidesshow(QString json);
    Q_INVOKABLE void queryAppStore(QString json);
    Q_INVOKABLE void queryEnOS(QString json);

    Q_INVOKABLE void installSopApp(QString path);
    Q_INVOKABLE void uninstallSopApp(QString sopId);
    Q_INVOKABLE void callPhone(QString json);
    Q_INVOKABLE void opensopApp(QString json);
    Q_INVOKABLE void changedPwd(QString json);
    Q_INVOKABLE void getSonOrgs(QString orgId);
    Q_INVOKABLE void updateAccount(QString json);
    Q_INVOKABLE void log(QString data);
    Q_INVOKABLE void getAccountInfo();
    Q_INVOKABLE void getAppLoginStatus();
    Q_INVOKABLE void getSystemAppList();
    Q_INVOKABLE void getLoginAuthCode(QString json);
signals:
    void voiceCall(QString param);
    void openApp(QString param);
    void callback(QString json);
    void noticeLastMsg(QString msgContent);
    void installApp(QString appPath);
    void uninstallApp(QString sopId);
    void getSystemApps();
//    void refreshData(QString json);
public slots:
    void onLoginAuthCodeResult(QString authCode);
    void onGetSystemApps(QString json);
    void onRefreshData(QString json);
    void onGetAppLoginStatusResult(QString json);
    void onLoginResult(QString json);
    void onLoginoutResult(QString json);
    void onPreLoginResult(QString json);
    void onGetAccountInfoResult(QString json);
    void onSlidesshowResult(QString json);
    void onQueryAppStoreResult(QString json,int type);
    void onQueryEnOSResult(QString json,int type);
    void onGetSonOrgsResult(QString json);
    void onChangedPwdResult(QString json);
    void onNoticeLastMsg(QString msgContent);
    void onUpdateAccountResult(QString json);

private slots:
    //void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager mNetMgr;
    QNetworkReply *mCurDownLoadReply;
    QFile mOutputFile;

    void initDBusConnect();
    void jsonParce(QString json,QString fName);
};

#endif // SOPSTORECLINET_H
