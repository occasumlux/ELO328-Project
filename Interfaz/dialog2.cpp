#include "dialog2.h"
#include "dialog3.h"
#include "ui_dialog2.h"
#include<iostream>
#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    if(video.isOpened())
    {

        video.release();
    }

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);




}
void Dialog2::on_pushButton_pressed()
{
    using namespace cv;

    if(video.isOpened())
    {
        ui->pushButton->setText("Iniciar");
        video.release();
        return;
    }
    bool isCamera;
    if(isCamera)
    {
        if(!video.open(0))
        {
            QMessageBox::critical(this,
                                  "Camera Error",
                                  "Make sure you entered a correct camera index,"
                                  "<br>or that the camera is not being accessed by another program!");
            return;
        }
    }



    ui->pushButton->setText("Stop");

    Mat frame;
    while(video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
        {

            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);
            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

    ui->pushButton->setText("Start");
}

void Dialog2::closeEvent(QCloseEvent *event)
{
    if(video.isOpened())
    {
        QMessageBox::warning(this,
                             "Warning",
                             "Stop the video before closing the application!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}



Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::on_push_resultado()
{
    Dialog3 *ventana3= new Dialog3(this);
    ventana3->setModal(true);
    ventana3->show();
}
