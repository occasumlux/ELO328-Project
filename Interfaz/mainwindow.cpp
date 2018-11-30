#include "mainwindow.h"
#include "ui_mainwindow.h"

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




void MainWindow::on_pushButton_clicked()
{
    Dialog1 *ventana1= new Dialog1(this);
    ventana1->setModal(true);
    ventana1->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    Dialog2 *ventana2= new Dialog2(this);
    ventana2->setModal(true);
    ventana2->show();
}
