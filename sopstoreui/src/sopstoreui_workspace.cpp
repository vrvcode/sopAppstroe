#include "sopstoreui_workspace.h"
#include "sopstoreclinet.h"
#include <QJsonObject>
#include <QJsonDocument>

int sopstoreui_Workspace::pageType(QString url)
{
    qDebug()<<Q_FUNC_INFO<<"page:"<<url;
    if((url.contains("/TimeOut"))||(url.contains("/App"))||url.endsWith("/Me")||url.contains("/Main")||url.contains("/Contacts")){
        return 0;
    }else if(url.contains("#/Quit")){//
        return -1;
    }else{
        return 1;
    }
}

void sopstoreui_Workspace::voiceCall(QString phone)
{
    QString call = "contact://showdialpage?"+phone;
    qDebug()<<Q_FUNC_INFO<<"call:"<<call;
    qApp->openUrl(call);
}

void sopstoreui_Workspace::install(QString params)
{
    qDebug()<<Q_FUNC_INFO<<"params:"<<params;
    mSysPkgMgr->install(params,false);
}

void sopstoreui_Workspace::uninstall(QString params)
{
    mSysPkgMgr->uninstall(params);
}

void sopstoreui_Workspace::openApp(QString url)
{
    if(url.contains("browser:")){
        mNeedNoticeRefreshData = true;
        CProcessManager proMgr;
        QList<int> pids = proMgr.processList();

        for(auto i : pids){
            qDebug()<<Q_FUNC_INFO<<"kill id:"<<i<<",sodId:"<<proMgr.sopidByPid(i);
            if(proMgr.sopidByPid(i) == "com.syberos.browser"){
                proMgr.killProcessByPid(i);
            }
        }
    }
    qApp->openUrl(url);
}

void sopstoreui_Workspace::getSystemAppList()
{
    QList<QSharedPointer<CPackageInfo> > list = mSysPkgMgr->packageInfoList();
    QJsonDocument doc;
    QJsonObject root;
    for(auto i :list){
        QJsonObject child;
        child.insert("ver",i->versionName());
        child.insert("name",i->name());
        root.insert(i->sopid(),child);
    }
    doc.setObject(root);
    QString json = doc.toJson();
    emit systemApps(json);
}

QUrl sopstoreui_Workspace::appUrl()
{
    QUrl url;
    url.setUrl("file:///data/apps/com.vrv.sopstore/bin/h5/index.html");
    return url;
}

sopstoreui_Workspace::sopstoreui_Workspace()
    : CWorkspace()
{
//serviceIP();
    mNeedNoticeRefreshData = false;
    qmlRegisterType<SopStoreClinet>("com.app.sopApp",1,0,"SopAppClient");

    mSysPkgMgr = QSharedPointer<CSystemPackageManager>(new CSystemPackageManager(this));
    connect(mSysPkgMgr.data(),SIGNAL(installStatusChanged(const QString &, const QString &, CPackageInfo::PackageStatus , CPackageInfo::PackageError , int )),this,SLOT(onInstallStatusChanged(const QString &, const QString &, CPackageInfo::PackageStatus , CPackageInfo::PackageError , int )));

    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->rootContext()->setContextProperty("mainApp", this);
    m_view->rootContext()->setContextProperty("globalApp", qApp);

    qApp->setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

sopstoreui_Workspace::~sopstoreui_Workspace()
{
    qDebug()<<Q_FUNC_INFO;
}

void sopstoreui_Workspace::onActive()
{
    //    if(mNeedNoticeRefreshData){
    //        mNeedNoticeRefreshData = false;
    //    }
    CProcessManager proMgr;
    QList<int> pids = proMgr.processList();
    for(auto i : pids){
        if(proMgr.sopidByPid(i) == "com.syberos.browser"){
            qDebug()<<Q_FUNC_INFO<<"kill browser.";
            proMgr.killProcessByPid(i);
        }
    }
    emit refreshData();
}

void sopstoreui_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    qApp->runService("sopstoreservice");
    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

void sopstoreui_Workspace::onInstallStatusChanged(const QString &sopid, const QString &pkgPath, CPackageInfo::PackageStatus status, CPackageInfo::PackageError error, int percent)
{

}

