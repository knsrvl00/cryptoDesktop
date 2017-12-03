#include "dialog.h"
#include "addcoindialog.h"
#include <QTableWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCoreApplication>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    coinList = new QTableWidget(this);
    coinList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    coinList->setRowCount(0);
    coinList->setColumnCount(5);
    coinList->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    coinList->setHorizontalHeaderItem(1, new QTableWidgetItem("Ticker"));
    coinList->setHorizontalHeaderItem(2, new QTableWidgetItem("Rank"));
    coinList->setHorizontalHeaderItem(3, new QTableWidgetItem("Price in USD"));
    coinList->setHorizontalHeaderItem(4, new QTableWidgetItem("Price in BTC"));
    addCoinButton = new QPushButton("Add New Coin", this);
    removeCoinButton = new QPushButton("Remove", this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTable()));
    //repeat updateTable() for every 10 mins
    timer->start(600000);

    connect(addCoinButton, SIGNAL(clicked()), this, SLOT(on_addCoinButton_clicked()));
    connect(removeCoinButton, SIGNAL(clicked()), this, SLOT(on_removeCoinButton_clicked()));
    connect(coinList, SIGNAL(cellClicked(int,int)), this, SLOT(selectRow(int,int)));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(addCoinButton);
    hlayout->addWidget(removeCoinButton);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addWidget(coinList);
    setLayout(vlayout);
}

void Dialog::on_addCoinButton_clicked(){
    addCoinDialog = new AddCoinDialog(this);
    addCoinDialog->show();
}

void Dialog::on_removeCoinButton_clicked(){
    QModelIndexList indexes =  coinList->selectionModel()->selectedRows();
    int countRow = indexes.count();

    for( int i = countRow; i > 0; i--)
        coinList->removeRow( indexes.at(i-1).row());
//    coinList->removeRow(coinList->currentRow());
}

void Dialog::addNewCoinToList(QString &ticker){

    QEventLoop eventLoop;

    QNetworkAccessManager man;
    QNetworkRequest req;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_0);
    req.setSslConfiguration(config);

    req.setUrl(QUrl(QString("https://api.coinmarketcap.com/v1/ticker/") + ticker + "/"));

    connect(&man, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkReply *reply = man.get(req);

    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
            //success
            QString strReply = (QString)reply->readAll();
            qDebug() << "Response:" << strReply;
            QJsonDocument doc = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonArray jsonArray = doc.array();
            for(int i = 0; i < jsonArray.size(); ++i){
                QJsonObject jsonObject = jsonArray[i].toObject();
                coinList->setRowCount(coinList->rowCount() + 1);
                coinList->setItem(coinList->rowCount() - 1, 0, new QTableWidgetItem(jsonObject["name"].toString()));
                coinList->setItem(coinList->rowCount() - 1, 1, new QTableWidgetItem(jsonObject["symbol"].toString()));
                coinList->setItem(coinList->rowCount() - 1, 2, new QTableWidgetItem(jsonObject["rank"].toString()));
                coinList->setItem(coinList->rowCount() - 1, 3, new QTableWidgetItem(jsonObject["price_usd"].toString()));
                coinList->setItem(coinList->rowCount() - 1, 4, new QTableWidgetItem(jsonObject["price_btc"].toString()));
            }

            delete reply;
        }
        else {
            //failure
            qDebug() << "Failure" <<reply->errorString();
            QMessageBox::information(this, tr("Error"), ticker + " not found");
            delete reply;
    }
}

void Dialog::selectRow(int x, int dummy){
    coinList->selectRow(x);
}

void Dialog::updateTable(){
    for (int i = 0; i < coinList->rowCount(); ++i){
        QTableWidgetItem *item = coinList->item(i ,0);
        QString coin = item->text();
        QEventLoop eventLoop;

        QNetworkAccessManager man;
        QNetworkRequest req;

        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1_0);
        req.setSslConfiguration(config);

        req.setUrl(QUrl(QString("https://api.coinmarketcap.com/v1/ticker/") + coin + "/"));

        connect(&man, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        QNetworkReply *reply = man.get(req);

        eventLoop.exec();

        if (reply->error() == QNetworkReply::NoError) {
                //success
                QString strReply = (QString)reply->readAll();
                qDebug() << "Response:" << strReply;
                QJsonDocument doc = QJsonDocument::fromJson(strReply.toUtf8());
                QJsonArray jsonArray = doc.array();
                QJsonObject jsonObject = jsonArray[0].toObject();
                coinList->setItem(i, 2, new QTableWidgetItem(jsonObject["rank"].toString()));
                coinList->setItem(i, 3, new QTableWidgetItem(jsonObject["price_usd"].toString()));
                coinList->setItem(i, 4, new QTableWidgetItem(jsonObject["price_btc"].toString()));
                delete reply;
            }
            else {
                //failure
                qDebug() << "Failure" <<reply->errorString();
                QMessageBox::information(this, tr("Error"), "coin not found");
                delete reply;
        }
    }
}

Dialog::~Dialog()
{

}
