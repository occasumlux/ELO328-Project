#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>
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

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = 0);
    ~Dialog1();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_pressed();

private:
    Ui::Dialog1 *ui;

    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
};

#endif // DIALOG1_H


