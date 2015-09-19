#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtSql/QtSql>
#include <QFile>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()

{
   delete ui;
}

void MainWindow::on_actionNew_Window_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), NULL, "sqllite3 (*.db)");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file_name);

    if(!db.open()){

        QMessageBox::information(this, "Title", "Can't connecte");
        quick_exit(0);

    }

    QSqlQuery query;
    query.exec("SELECT Name from ChatInfo");

    QSqlQueryModel * modal = new QSqlQueryModel();
    modal->setQuery(query);
    ui->comboBox->setModel(modal);


}
void MainWindow::on_actionClose_triggered()
{
    quick_exit(0);
}


void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    QString chat_text = ui->comboBox->currentText();
    QSqlQuery getchat;
    getchat.exec("SELECT MessageType,Body from EventInfo where ChatToken='"+chat_text+"'");
    int num_rows;
    num_rows = getchat.value(0).toInt();
    QSqlQueryModel * modal1 = new QSqlQueryModel();
    modal1->setQuery(getchat);
    ui->tableView->setModel(modal1);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}

void MainWindow::on_pushButton_2_clicked()
{

    if(!QSqlDatabase::database().isOpen())
    {
        QMessageBox::information(this, "title", "First of all make sure to connect the db file");
    }else{
       QString chat_text = ui->comboBox->currentText();
       QString filename= chat_text;
       QFile file( filename );
       file.open(QIODevice::ReadWrite);
       QTextStream stream( &file );
    QSqlQuery getchat;
    QSqlQuery owner;

    QSqlQuery getchatid;
    getchatid.exec("SELECT ChatID from ChatInfo");
    getchatid.next();
    QString chatidlol = getchatid.value(0).toString();

    QString viberowner1;
    qDebug() << chatidlol;
    owner.exec("SELECT Number from ChatRelation where ChatID='"+chatidlol+"'");
    owner.next();
    viberowner1 = owner.value(0).toString();

    while( getchat.next()){

    QString country = getchat.value(0).toString();
    country.replace("65", viberowner1);
    country.replace("72", chat_text);


    QString country1 = getchat.value(1).toString();
    stream <<  country <<  ",";
    stream <<  country1  << endl;

    }

    QMessageBox::information(this, "Save as file", "Data Successfully saved!");

}
}

void MainWindow::on_actionContact_triggered()
{
    QSqlQuery getchat;
    getchat.exec("select  cr.Number,  c.FirstName  from ContactRelation cr left join  Contact c on cr.ContactID = c.ContactID order by cr.Number;");
    int num_rows;
    num_rows = getchat.value(0).toInt();
    QSqlQueryModel * modal1 = new QSqlQueryModel();
    modal1->setQuery(getchat);
    ui->tableView->setModel(modal1);
}
