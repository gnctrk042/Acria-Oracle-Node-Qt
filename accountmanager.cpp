#include "accountmanager.h"
#include "ui_accountmanager.h"

#include <QFileInfo>
#include <QWebChannel>

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
}

AccountManager::AccountManager(QWidget *parent, Data* _data) :
    QDialog(parent),
    ui(new Ui::AccountManager)
{
    ui->setupUi(this);

    this->data = _data;

    QWebChannel * channel = new QWebChannel(this->ui->webEngineView->page());
    this->ui->webEngineView->page()->setWebChannel(channel);
    this->ui->webEngineView->page()->webChannel()->registerObject(QStringLiteral("AccountManager"), this);

    this->ui->webEngineView->page()->setBackgroundColor(Qt::transparent);
    this->ui->webEngineView->setUrl(QUrl::fromLocalFile(QFileInfo("js/index.html").absoluteFilePath()));
    this->ui->webEngineView->setZoomFactor(0.4);

    delay();
    if(this->data->binance_wallet_path.length() > 4)
    this->ui->webEngineView->page()->runJavaScript("binWalletPath = " + this->data->binance_wallet_path + ";document.getElementById(\"selectWalletBin\").innerHTML = \"...\" + binWalletPath.substr(binWalletPath.length - 20);");
    qDebug() << this->data->eth_wallet_path;
    if(this->data->eth_wallet_path.length() > 4)
    this->ui->webEngineView->page()->runJavaScript("ethWalletPath = '" + this->data->eth_wallet_path + "';document.getElementById('selectWalletEth').innerHTML = '...' + ethWalletPath.substr(ethWalletPath.length - 20);");

    /*
    QFile apiFile(":/qtwebchannel/qwebchannel.js"); //load the API from the resources
    if(!apiFile.open(QIODevice::ReadOnly))
        qDebug()<<"Couldn't load Qt's QWebChannel API!";
    QString apiScript = QString::fromLatin1(apiFile.readAll());
    qDebug() << apiScript;
    apiFile.close();
    this->ui->webEngineView->page()->runJavaScript(apiScript);

    std::ofstream webchannel ("./js/qwebchannel.js");

    if (webchannel.is_open())
    {
      webchannel << apiScript.toStdString();
      webchannel.close();
    }
    */


}

AccountManager::~AccountManager()
{
    delete ui;
}

void AccountManager::on_buttonBox_accepted()
{
    this->ui->webEngineView->page()->runJavaScript("ethPrivateKey", [this](const QVariant &v) { qDebug() << v.toString(); this->data->eth_private_key = v.toString();this->data->changed = true;});
    this->ui->webEngineView->page()->runJavaScript("ethAddress", [this](const QVariant &v) { qDebug() << v.toString(); this->data->eth_account = v.toString();this->data->changed = true;});
    this->ui->webEngineView->page()->runJavaScript("ethWalletPath", [this](const QVariant &v) { qDebug() << v.toString(); this->data->eth_wallet_path = v.toString();this->data->changed = true;});

    this->ui->webEngineView->page()->runJavaScript("binPrivateKey", [this](const QVariant &v) { qDebug() << v.toString(); this->data->binance_private_key = v.toString();this->data->changed = true;});
    this->ui->webEngineView->page()->runJavaScript("binAddress", [this](const QVariant &v) { qDebug() << v.toString(); this->data->binance_account = v.toString();this->data->changed = true;});
    this->ui->webEngineView->page()->runJavaScript("binWalletPath", [this](const QVariant &v) { qDebug() << v.toString(); this->data->binance_wallet_path = v.toString();this->data->changed = true;});

    delay();
}
