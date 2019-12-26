#ifndef VIDEOWINDOWS_H
#define VIDEOWINDOWS_H

#include "funcwindow.h"

class videoWindows : public FuncWindow
{
    Q_OBJECT
public:
    videoWindows(QWidget *parent = nullptr);
    ~videoWindows();

public slots:
    //重写以下三个函数即可
//    //上一首
//    void prevPlay();
//    //下一首
//    void nextPlay();

//     //读取文件路径
//    void readFileName();

private:

};

#endif // VIDEOWINDOWS_H
