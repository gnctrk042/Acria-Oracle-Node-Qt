#include "infogeth.h"
#include "ui_infogeth.h"

InfoGeth::InfoGeth(QWidget *parent, Node* _node, Data *_data, QString _type) :
    QDialog(parent),
    ui(new Ui::InfoGeth)
{
    ui->setupUi(this);

    this->node = _node;
    this->data = _data;

    this->update_info();

    this->type = _type;
}

InfoGeth::~InfoGeth()
{
    delete ui;
}

void InfoGeth::update_info(){
    QString version1 = "";
    QString url1 = "";
    std::vector<QString> items;

    version1 = node->get_geth_version();

    if(this->type == "ethereum"){
        url1 = this->data->geth_url;
        items = data->items;
    }
    else if(this->type == "binance"){
        url1 = this->data->binance_url;
        items = data->binance_items;
    }

    this->ui->label_version->setText(version1);
    this->ui->label_url->setText(url1);

    QString tmp;

    for(uint i = 0; i<items.size();i++){
        tmp+=items[i];
        tmp+="\n";
    }

    this->ui->label_items->setText(tmp);
}
