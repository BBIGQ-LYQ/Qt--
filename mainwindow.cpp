#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videowindows.h"
#include "musicwindows.h"

//#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化视频窗体
    videoWin = new videoWindows(this);
    videoWin->setStyleSheet(QStringLiteral("FuncWindow{background-image: url(:/pic/one.jpeg);}"));

    //初始化音乐窗体
    musicWin = new musicWindows(this);
    musicWin->setStyleSheet(QStringLiteral("FuncWindow{background-image: url(:/pic/music.jpeg);}"));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    videoWin->show();
    videoWin->readFileName();
    this->hide();
}

void MainWindow::on_pushButton_2_clicked()
{
    musicWin->show();
    musicWin->readFileName();
    this->hide();
}
