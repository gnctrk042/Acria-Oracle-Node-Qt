#include "configitem.h"
#include "ui_configitem.h"
#include "json.hpp"

ConfigItem::ConfigItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigItem)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);

    t1.push_back(this->ui->lineEdit_json1);
    t1.push_back(this->ui->lineEdit_json2);
    t1.push_back(this->ui->lineEdit_json3);
    t1.push_back(this->ui->lineEdit_json4);
}

ConfigItem::~ConfigItem()
{
    delete ui;
}

void ConfigItem::clear(){
    this->ui->lineEdit_2_api_url->setText("");
    this->ui->lineEdit_resource_name->setText("");

    for(uint i = 0;i<this->t1.size();i++){
        t1[i]->setText("");
    }
}

void ConfigItem::on_buttonBox_1_accepted()
{
    nlohmann::json tmp;
    tmp["url"] = this->ui->lineEdit_2_api_url->text().toStdString();
    tmp["url_data"] = this->ui->lineEdit_2_api_url_2->text().toStdString();
    tmp["parameter_type"] = this->ui->lineEdit_2_parameter_type->text().toStdString();
    tmp["rname"] = this->ui->lineEdit_resource_name->text().toStdString();

    for(uint i = 0;i<this->t1.size();i++){
        if(this->t1[i]->text().trimmed() != "")
        tmp["json"][i] = this->t1[i]->text().toStdString();
    }

    ijson = tmp;

    this->clear();
}

void ConfigItem::on_buttonBox_1_rejected()
{

}

void ConfigItem::on_lineEdit_resource_name_textChanged(const QString &arg1)
{
    if(arg1.length()>32){
        static_cast<QLineEdit*>(sender())->setText(arg1.mid(0, 32));
    }
}
