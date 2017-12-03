#include "addcoindialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QDebug>
#include <Qtimer>
#include <QMessageBox>

AddCoinDialog::AddCoinDialog(QWidget *parent) : QDialog(parent){
    label = new QLabel("Name", this);
    lineEdit = new QLineEdit;
    addButton = new QPushButton("Add", this);
    cancelButton = new QPushButton("Cancel", this);

    setWindowTitle("Add New Coin");
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(this, SIGNAL(sendTickerToParent(QString &)), parent, SLOT(addNewCoinToList(QString &)));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(lineEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(addButton);
    bottomLayout->addWidget(cancelButton);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(topLayout);
    vlayout->addLayout(bottomLayout);

    setLayout(vlayout);
}

void AddCoinDialog::on_addButton_clicked(){
    QString ticker = lineEdit->text();

    if (ticker.isEmpty()){
        QMessageBox::information(this, tr("Coin Chart"), tr("Please enter a valid coin name. Use (*) to add all coins."));
    }
    //if ticker is * then add all coins
    else if (ticker == "*"){
        QString null = "";
        emit sendTickerToParent(null);
    }
    else
        emit sendTickerToParent(ticker);

    delete this;
}


