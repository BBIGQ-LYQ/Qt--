#ifndef FUNCWINDOW_H
#define FUNCWINDOW_H

#include <QMainWindow>

#include <QListWidgetItem>
//鼠标事件重写
#include <QMouseEvent>
#include <QInputEvent>
#include <QEvent>
//进程头文件
#include <QProcess>
//定时器
#include <QTimer>

//工具栏头文件
#include "toolswindow.h"

//读取文件
#include <QDir>
#include <QStringList>

//linux: /home/bbigq/workspace/QT/ARM_player/mat/
//arm:/work/qtwork/mat/
//nfs:/work/nfs/ARM_player/mat/
#define FilePath "/home/bbigq/workspace/QT/ARM_player/mat/"

namespace Ui {
class FuncWindow;
}

class FuncWindow : public QMainWindow
{
    Q_OBJECT

public:
    //工具栏
    ToolsWindow * toolwin;
    //视频 1/音乐 2
    int variety = 0;
    //工具表触发状态  0为未触发  1为触发
    int state = 1;
    //播放状态
    int playTime = 0;
    //播放器进程
    QProcess *mPro;
    //当前播放的文件
    QString nowFile = "";
    //当前事件总时间
    double allTime = 0;
    //当前进度(百分比)
    int precent = 0;
    //当前时间
    double preTime = 0;
    //当前音量值
    int volume = 50;
    //定时器
    QTimer *timer;
    //文件列表
    QStringList fileList;

    explicit FuncWindow(QWidget *parent = nullptr);
    ~FuncWindow();

    //重写单击触发文件列表
    void mousePressEvent(QMouseEvent *event);

signals:
    void reflashRate(int);

public slots:
    //播放
    virtual void play();
    //上一首
    virtual void prevPlay();
    //下一首
    virtual void nextPlay();
    //快退
    virtual void quickRetreat();
    //快进
    virtual void fastForward();
    //音量加
    virtual void VolumePlus();
    //音量减
    virtual void volumeDN();
    //推出当前界面
    virtual void out();
    //获取当前进度
    virtual void getRate();
    //读取当前播放信息
    virtual void readData();
    //读取文件路径
    virtual void readFileName();
    //刷新链表
    virtual void refreshList();
    //停止定时器
    virtual void stopTimer();
    //跳转到目标
    virtual void jumpAim(int);
    //播放前数据准备
    virtual void readyData(){}
    //播放结束回调
    virtual void overHandle();


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::FuncWindow *ui;
};

#endif // FUNCWINDOW_H
