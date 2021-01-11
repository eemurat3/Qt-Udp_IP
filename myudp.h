#ifndef MYUDP_H
#define MYUDP_H

#include <iostream>
#include <QMainWindow>
#include <QMainWindow>
#include <QUdpSocket>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDir>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QCheckBox>
#include <QButtonGroup>
#include <QComboBox>
#include <QFile>
#include<QTextEdit>
#include <QDateTime>
#include <QValidator>
#include <QMessageBox>

#include "logger.h"
#include <iostream>
#include <string>
#include <QFileDialog>

using namespace std;

namespace Ui {
class MyUdP;
}

class MyUdP : public QMainWindow
{
    Q_OBJECT


public:
    explicit MyUdP(QWidget *parent = 0);
    ~MyUdP();
    void message();
    void saveFile();

public slots:
    //void readyRead();
    void accept();//for dialog scrreen
    void readyRead();
    void sendButton();
    void saveButton1();
    void saveButton2();

private:
    //Ui::MyUdP *ui;

    QByteArray Buffer;//for read message data
    QLineEdit *lineMessage;
    QLineEdit *portname;

    QUdpSocket * socket;
    bool SocketBind;

    //main window setup
    void Setup();

    QTabWidget *mainTab;
    QWidget *mainWidget;
    QVBoxLayout *vboxTabWidget;


    /*for tab1 Screen*/
    void setTab1();
    QWidget *tab1;
    QTextEdit *inputMessage;
    QTextEdit *receiveMessage;

    /*tab screen1*/

    /*tab screen 2*/
    void setTab2();
    QWidget *tab2;
    bool boolSaveReceive;
    bool boolSaveSend;
    Logger *loggerReceive;
    Logger *loggerSend;


    /*tab screen 2*/


    //for dialog screen
    void Dialog();
    QDialog *dialog;
    QDialogButtonBox *buttonBox;
    QLabel *info;
    QLineEdit *lineServer;
    QLineEdit *linePort;
    //dialog screen


};

#endif // MYUDP_H
