#include "node.h"
#include "util.h"

#include <QString>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>


Node::Node(Data* _data, QString _type)
{
    this->data = _data;
    this->type = _type;

    status_manager = new QNetworkAccessManager();

    QObject::connect(status_manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(statusManagerFinished(QNetworkReply*)));

    this->status_geth = false;
    this->status_polkadot = true;
    this->status_acria = true;
    this->status_config = true;

    update_geth_status();
};

void Node::update_geth_status(){
    QUrl url1;

    if(this->type == "ethereum")
        url1 = QUrl(this->data->geth_url);

    if(this->type == "binance")
        url1 = QUrl(this->data->binance_url);

    qDebug() << "answer";
    QJsonObject obj;
    obj["jsonrpc"] = "2.0";
    obj["method"] = "web3_clientVersion";
    obj["params"] = "";
    obj["id"] = 67;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    status_request.setUrl(url1);
    status_request.setRawHeader("Content-Type", "application/json");
    status_manager->post(status_request, data);
}

Node::~Node()
{
    delete status_manager;
};

void Node::statusManagerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "error " << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    qDebug() << "answer " <<answer;

    QJsonObject obj = ObjectFromString(answer);

    qDebug() << obj["result"].toString();

    this->geth_version = obj["result"].toString();

    status_geth = true;
}

bool Node::get_status_geth(){
    return status_geth;
}

bool Node::get_status_polkadot(){
    return status_polkadot;
}

bool Node::get_status_acria(){
    return status_acria;
}

bool Node::get_status_config(){
    return status_config;
}
