#ifndef ADDCOINDIALOG_H
#define ADDCOINDIALOG_H

#include <QDialog>

class QPushButton;
class QLabel;
class QLineEdit;

class AddCoinDialog : public QDialog{
    Q_OBJECT
public:
    AddCoinDialog::AddCoinDialog(QWidget *parent = 0);
signals:
    void sendTickerToParent(QString &);
public slots:
    void on_addButton_clicked();
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *cancelButton;
};

#endif // ADDCOINDIALOG_H
