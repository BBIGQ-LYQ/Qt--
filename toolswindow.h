#ifndef TOOLSWINDOW_H
#define TOOLSWINDOW_H
#include <QWidget>
#include <QDebug>
namespace Ui {
class ToolsWindow;
}

class ToolsWindow : public QWidget
{
    Q_OBJECT

public:


    explicit ToolsWindow(QWidget *parent = nullptr);
    ~ToolsWindow();

//自定义信号
signals:
    void PLAYSIG();         //执行音频文件类
    void PREVSIG();         //上一首/个
    void NEXTSIG();         //下一首/个
    void MOVEPRE();         //快进
    void RETURNB();         //倒退
    void VOLADD();          //音量加
    void VOLSUB();          //音量减
    void STOPTIMER();       //停止定时器
    void SKIP(int);         //滑动进度条
    void OUT();             //退出

public slots:
    void reflashRateRealTime(int ); //刷新进度条

private slots:
    void on_but_out_clicked();      //退出按钮被点击

    void on_but_rev_clicked();      //倒退按钮被点击

    void on_but_frev_clicked();     //上一首按钮被点击

    void on_but_play_clicked();     //播放按钮被点击

    void on_but_next_clicked();     //下一首按钮被点击

    void on_but_adv_clicked();      //快进按钮被点击

    void on_but_volup_clicked();    //音量加按钮被点击

    void on_but_voldown_clicked();  //阴凉减按钮被点击

    void on_precent_seek_sliderPressed();//进度条被按下

    void on_precent_seek_sliderReleased();//进度条松开

private:
    Ui::ToolsWindow *ui;
};

#endif // TOOLSWINDOW_H
