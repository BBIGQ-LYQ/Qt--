#ifndef MUSICWINDOWS_H
#define MUSICWINDOWS_H

#include "funcwindow.h"
//读取文件
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QListWidget>
#include <QLabel>

class musicWindows : public FuncWindow
{
    Q_OBJECT
public:
    QListWidget *listWidget;
    QLabel *Title;
    //歌词列表
    QStringList lrcTimeList;

    musicWindows(QWidget *parent = nullptr);
    ~musicWindows();

    void out();
    //获取歌词
    void getLrc();
    //打印歌词
    void showLrc();


public slots:

    //读取数据并处理
    void readData();
    //数据准备
    void readyData();

private:

};

#endif // MUSICWINDOWS_H
