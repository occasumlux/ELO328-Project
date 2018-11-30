#ifndef DIALOG2_H
#define DIALOG2_H

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
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_pressed();
    void on_push_resultado();
private:
    Ui::Dialog2 *ui;

    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
};

#endif // DIALOG2_H
