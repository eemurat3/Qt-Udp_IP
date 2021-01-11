#include "myudp.h"
#include "ui_myudp.h"
#include "logger.h"

MyUdP::MyUdP(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MyUdP)
{
    //ui->setupUi(this);
    Dialog();
    Setup();
}

MyUdP::~MyUdP()
{
    //delete ui;
    qDebug()<<"deconstructor is  working";
}

void MyUdP::Setup()
{
    /***design mainWindow***/
    qDebug()<<"constructor";
    cout<<"1 2 3 4";
    mainWidget=new QWidget();
    mainTab=new QTabWidget();
    tab1=new QWidget();
    tab2=new QWidget();

    QLabel *portno=new QLabel("Connection Port Number   :");
    QLineEdit *port=new QLineEdit();
    port->setText(linePort->text());
    port->setReadOnly(true);

    QLabel *ipAdress=new QLabel("Connection IP Adress           :");
    QLineEdit *ip=new QLineEdit();
    ip->setText(lineServer->text());
    ip->setReadOnly(true);

    QHBoxLayout *h1=new QHBoxLayout();
    h1->addWidget(portno);
    h1->addWidget(port);

    QHBoxLayout *h2=new QHBoxLayout();
    h2->addWidget(ipAdress);
    h2->addWidget(ip);

    setTab1();
    setTab2();
    mainTab->addTab(tab1,QString("Data"));
    mainTab->addTab(tab2,QString("Log"));

    QVBoxLayout *vboxTabWidget=new QVBoxLayout();
    vboxTabWidget->addLayout(h2);
    vboxTabWidget->addLayout(h1);

    vboxTabWidget->addWidget(mainTab);

    mainWidget->setLayout(vboxTabWidget);
    mainWidget->show();

    /*design mainWindow*/
    socket= new QUdpSocket(this);
    socket->bind( QHostAddress(lineServer->text()),QString(linePort->text()).toInt());
    //socket->bind(QHostAddress::LocalHost, 7755);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    qDebug()<<"socket connect";
}

void MyUdP::setTab1()
{
    qDebug()<<" 1 2 3 4";
    QPushButton *sendMessage=new QPushButton("send message");
    sendMessage->setStyleSheet("background-color: green");
    QLabel *labelMessage=new QLabel("                                        Input Message");

    inputMessage = new QTextEdit();
    inputMessage->setDocumentTitle("Input Message");
    inputMessage->createStandardContextMenu();

    QVBoxLayout *vbox=new QVBoxLayout();
    vbox->addWidget(labelMessage);
    vbox->addWidget(inputMessage);
    vbox->addWidget(sendMessage);

    QLabel *receiveMessageLabel=new QLabel("                                                   Receive Message");
    receiveMessage = new QTextEdit();
    receiveMessage->setReadOnly(true);
    receiveMessage->createStandardContextMenu();

    QVBoxLayout *vbox1=new QVBoxLayout();
    vbox1->addWidget(receiveMessageLabel);
    vbox1->addWidget(receiveMessage);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(vbox);
    hbox->addLayout(vbox1);

    connect(sendMessage,SIGNAL(released()),this,SLOT(sendButton()));

    tab1->setLayout(hbox);

}

void MyUdP::readyRead()
{
    qDebug()<<"ready read";

    while (socket->hasPendingDatagrams()){
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);
    receiveMessage->setText(QString(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss:z")+" -> "+Buffer.toStdString().c_str()));
    loggerReceive->write(receiveMessage->toPlainText()+"\n");
    }

}

void MyUdP::message(){
    qDebug()<<"message function";
    qDebug()<<"local host:"<<socket->localAddress().toString();
    QByteArray Data;
    Data.append(inputMessage->toPlainText());
    socket->writeDatagram(Data,QHostAddress(lineServer->text()),
                          QString(linePort->text()).toInt());
    loggerSend->write(QString(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss:z")+ "->" + Data.toStdString().c_str()+"\n"));
}
void MyUdP::sendButton(){
    message();
}



void MyUdP::setTab2()
{

    QPushButton *saveButton=new QPushButton("save file");
    saveButton->setStyleSheet("background-color: blue");

    QPushButton *saveButton1=new QPushButton("save File");
    saveButton1->setStyleSheet("background-color: green");

    //for receive message field
    QLabel *labelReceive=new QLabel("Receive log");
    QPlainTextEdit *editorReceive = new QPlainTextEdit(this);
    editorReceive->setReadOnly(true);
    QString fileName = "receive.txt";
    loggerReceive = new Logger(this, fileName, editorReceive);

    //for send message field
    QLabel *labelSend=new QLabel("Send log");
    QPlainTextEdit *editorSend = new QPlainTextEdit(this);
    editorSend->setReadOnly(true);
    QString fileName1 = "send.txt";
    loggerSend = new Logger(this, fileName1, editorSend);

    //tab3 design
    QVBoxLayout *vbox1=new QVBoxLayout();
    vbox1->addWidget(labelReceive);
    vbox1->addWidget(editorReceive);
    vbox1->addWidget(saveButton1);

    QVBoxLayout *vbox2=new QVBoxLayout();
    vbox2->addWidget(labelSend);
    vbox2->addWidget(editorSend);
    vbox2->addWidget(saveButton);

    QHBoxLayout *hbox1=new QHBoxLayout();
    hbox1->addLayout(vbox2);
    hbox1->addLayout(vbox1);

    tab2->setLayout(hbox1);

     connect(saveButton1,SIGNAL(released()),this,SLOT(saveButton1()));
     connect(saveButton,SIGNAL(released()),this,SLOT(saveButton2()));
}

void MyUdP::saveButton1()
{
    boolSaveReceive=true;
    saveFile();
}

void MyUdP::saveButton2()
{
    boolSaveSend=true;
    saveFile();
}


void MyUdP::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Address Book"), "",
            tr("Address Book (*.abk);;All Files (*)"));
    if (fileName.isEmpty())
        return;
     else{
         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly)) {

              QMessageBox::information(this, tr("Unable to open file"),
              file.errorString());
               return;
          }

         QTextStream out(&file);
         out.setCodec("UTF-8");
         if(boolSaveReceive){
         out <<loggerReceive->m_editor->toPlainText();
         boolSaveReceive=false;
         }

         if(boolSaveSend){
             out <<loggerSend->m_editor->toPlainText();
             boolSaveSend=false;

         }
     }
 }


/****************for Dialog Screen**************/
void MyUdP::Dialog()
{
     qDebug()<<"dialog";

    dialog= new QDialog(this);
    dialog->setWindowTitle(QString("input port name"));

    info = new QLabel(" ");
    QLabel *labelServer=new QLabel("Server Address   ");
    QLabel *labelPort=new QLabel("Port                        ");

    lineServer=new QLineEdit(dialog);
    //lineServer->setInputMask("xxx.xxx.xxx.xxx");
    lineServer->setPlaceholderText("ex(127.0.0.1)");
    lineServer->setClearButtonEnabled(true);


    QHBoxLayout *hbox=new QHBoxLayout();
    hbox->addWidget(labelServer);
    hbox->addWidget(lineServer);


    linePort=new QLineEdit(dialog);
    QValidator *portValidator = new QIntValidator(0, 65535, this);
    linePort->setPlaceholderText("1 to 65535");
    linePort->setValidator(portValidator);
    linePort->setMaxLength(5);
    linePort->setClearButtonEnabled(true);


    QHBoxLayout *hbox1=new QHBoxLayout();
    hbox1->addWidget(labelPort);
    hbox1->addWidget(linePort);

    QVBoxLayout vbox1(dialog);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
                                    // | QDialogButtonBox::Cancel);
    vbox1.addWidget(info);
    vbox1.addLayout(hbox);
    vbox1.addLayout(hbox1);
    vbox1.addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    //connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    dialog->exec();

}

void MyUdP::accept()
{
    if(!(lineServer->text().isEmpty()) && !(linePort->text().isEmpty()))
    {

        qDebug()<<"Server address and port number are saved";
        dialog->close();
    }
    else
    {
        qDebug()<<"server address or port number is empty";
        info->setText("server address or port number is empty");
    }
}
/****************for Dialog Screen**************/
