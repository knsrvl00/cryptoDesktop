#ifndef DIALOG_H
#define DIALOG_H

#include "addcoindialog.h"
#include <QDialog>

class QTableWidget;
class QPushButton;
class QNetworkReply;
class QTimer;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void on_addCoinButton_clicked();
    void on_removeCoinButton_clicked();
    void addNewCoinToList(QString &);
    void updateTable();
    void selectRow(int, int);
private:
    AddCoinDialog *addCoinDialog;
    QTableWidget *coinList;
    QPushButton *addCoinButton;
    QPushButton *removeCoinButton;
    QTimer *timer;
};

#endif // DIALOG_H
