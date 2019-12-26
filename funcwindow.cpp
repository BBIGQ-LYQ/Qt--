#include "funcwindow.h"
#include "ui_funcwindow.h"
#include <QDebug>
FuncWindow::FuncWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FuncWindow)
{
    ui->setupUi(this);

    //工具栏窗体初始化
    toolwin = new ToolsWindow(this);
    toolwin->setGeometry(0, 410, toolwin->width(), toolwin->height());
    toolwin->stackUnder(ui->listWidget);
    toolwin->show();

    //监听信号
    connect(toolwin, SIGNAL(PLAYSIG()), this, SLOT(play()));
    connect(toolwin, SIGNAL(PREVSIG()), this, SLOT(prevPlay()));
    connect(toolwin, SIGNAL(NEXTSIG()), this, SLOT(nextPlay()));
    connect(toolwin, SIGNAL(MOVEPRE()), this, SLOT(fastForward()));
    connect(toolwin, SIGNAL(RETURNB()), this, SLOT(quickRetreat()));
    connect(toolwin, SIGNAL(VOLADD()), this, SLOT(VolumePlus()));
    connect(toolwin, SIGNAL(STOPTIMER()), this, SLOT(stopTimer()));
    connect(toolwin, SIGNAL(SKIP(int)), this, SLOT(jumpAim(int)));
    connect(toolwin, SIGNAL(VOLSUB()), this, SLOT(volumeDN()));
    connect(toolwin, SIGNAL(OUT()), this, SLOT(out()));

    //创建新进程
    mPro = new QProcess(this);

    //关联进程可读信号
    connect(mPro,SIGNAL(readyRead()),this,SLOT(readData()));
    //关联进程结束信号
    connect(mPro, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(overHandle()));

    timer = new QTimer(this);
    //更新进度条
    connect(timer,SIGNAL(timeout()), this, SLOT(getRate()));
}

FuncWindow::~FuncWindow()
{
    delete ui;
}

//重写鼠标单击事件
void FuncWindow::mousePressEvent(QMouseEvent *event)
{
    //获取点击时的时间
    getRate();
    if(state == 1)
    {
        if(event->x() < 600 && event->y() < 400)
        {
            state = 0;

            mPro->kill();
            mPro->waitForFinished();
            QString command;
            if(nowFile.indexOf(".mp4") > -1)
            {
                command = "mplayer  -slave  -quiet     -zoom    -x 800  -y  480 ";
            }
            else if(nowFile.indexOf(".mp3") > -1)
            {
                command = "mplayer  -slave  -quiet ";
            }
            mPro->start(command + FilePath + nowFile);
            jumpAim(precent);
            ui->listWidget->hide();
            toolwin->hide();
        }
    }
    else if(state == 0)
    {
        state = 1;
        ui->listWidget->raise();
        toolwin->raise();

        ui->listWidget->show();
        toolwin->show();

        mPro->kill();
        mPro->waitForFinished();
        if(nowFile != "")
        {
            QString command;
            if(nowFile.indexOf(".mp4") > -1)
            {
                command = "mplayer  -slave  -quiet     -zoom    -x 600  -y  400 ";
            }
            else if(nowFile.indexOf(".mp3") > -1)
            {
                command = "mplayer  -slave  -quiet ";
            }
            mPro->start(command + FilePath + nowFile);
            jumpAim(precent);
        }
    }

}

//0是未播状态   1是播放状态   2是暂停态
void FuncWindow::play()
{
    //处于未播放状态时，转播放状态
    if(playTime != 0)
    {
        if(playTime == 1)   //播放状态转暂停
        {
            timer->stop();
            mPro->write("pause \n");
            playTime = 2;
        }
        else if(playTime == 2)
        {
            playTime = 1;
            mPro->write("pause \n");
            timer->start(500);
        }
    }
}

void FuncWindow::prevPlay()
{
    int now = 0;
    if(variety == 1)
    {
        if(nowFile != "")
        {
            for(int i = 0; i < fileList.size(); i++)
            {
                if(fileList.at(i) == nowFile)
                {
                    mPro->kill();
                    mPro->waitForFinished();
                    if(i == 0)
                    {
                        now = fileList.size() - 1;
                        nowFile = fileList.at(now);
                    }
                    else
                    {
                        nowFile = fileList.at(i - 1);
                        now = i - 1;
                    }
                    playTime = 1;
                    QString command = "mplayer  -slave  -quiet     -zoom    -x 600  -y  400 ";
                    mPro->start(command + FilePath + nowFile);
                    mPro->write("get_time_length \n");
                    break;
                }
            }
        }
    }
    else if(variety == 2)
    {
        if(nowFile != "")
        {
            qDebug() << "prevent touch" ;
            for(int i = 0; i < fileList.size(); i++)
            {
                qDebug() << fileList.at(i);
                if(fileList.at(i) == nowFile)
                {
                    mPro->kill();
                    mPro->waitForFinished();
                    if(i == 0)
                    {
                        now = fileList.size() - 1;
                        nowFile = fileList.at(now);
                    }
                    else
                    {
                        nowFile = fileList.at(i - 1);
                        now = i - 1;
                    }
                    playTime = 1;
                    readyData();
                    QString command = "mplayer  -slave  -quiet ";
                    mPro->start(command + FilePath + nowFile);
                    mPro->write("get_time_length \n");
                    qDebug() << command + FilePath + nowFile ;
                    break;
                }
            }
        }
    }
    ui->listWidget->setCurrentRow(now);
    ui->listWidget->scrollToItem(ui->listWidget->currentItem(), QAbstractItemView::PositionAtCenter);

}

void FuncWindow::nextPlay()
{
    int now = 0;
    if(variety == 1)
    {
        if(nowFile != "")
        {
            for(int i = 0; i < fileList.size(); i++)
            {
                qDebug() << "list name" << fileList.at(i);
                qDebug() << "now file" << nowFile;
                if(fileList.at(i) == nowFile)
                {
                    if(playTime != 0)
                    {
                        mPro->kill();
                        mPro->waitForFinished();
                    }

                    if(i == fileList.size()-1)
                    {
                        nowFile = fileList.at(0);
                        now = 0;
                    }
                    else
                    {
                        nowFile = fileList.at(i + 1);
                        now = i+1;
                    }
                    playTime = 1;
                    QString command = "mplayer  -slave  -quiet     -zoom    -x 600  -y  400 ";
                    mPro->start(command + FilePath + nowFile);

                    mPro->write("get_time_length \n");

                    break;
                }
            }
        }
    }
    else if(variety == 2)
    {
        if(nowFile != "")
        {
            for(int i = 0; i < fileList.size(); i++)
            {
                if(fileList.at(i) == nowFile)
                {
                    if(playTime != 0)
                    {
                        mPro->kill();
                        mPro->waitForFinished();
                    }

                    if(i == fileList.size()-1)
                    {
                        nowFile = fileList.at(0);
                        now = 0;
                    }
                    else
                    {
                        nowFile = fileList.at(i + 1);
                        now = i+1;
                    }
                    playTime = 1;
                    readyData();
                    QString command = "mplayer  -slave  -quiet ";
                    mPro->start(command + FilePath + nowFile);

                    mPro->write("get_time_length \n");

                    break;
                }
            }
        }
    }

    ui->listWidget->setCurrentRow(now);
    ui->listWidget->scrollToItem(ui->listWidget->currentItem(), QAbstractItemView::PositionAtCenter);

}

void FuncWindow::quickRetreat()
{
    mPro->write("seek -5\n");
}

void FuncWindow::fastForward()
{
    mPro->write("seek +5\n");
}

void FuncWindow::VolumePlus()
{
    if(volume <= 95)
        mPro->write("volume +5\n");
    else
        mPro->write("volume 100 1\n");
}

void FuncWindow::volumeDN()
{
    if(volume >= 5)
        mPro->write("volume -5\n");
    else
        mPro->write("volume 0 1\n");
}

void FuncWindow::out()
{
        mPro->kill();
        mPro->waitForFinished();
//        //退出播放器
//        mPro->write("quit 0\n");
        //停止定时器
        timer->stop();

        //所有参数复位
        state = 1;
        playTime = 0;
        allTime = 0;
        precent = 0;
        nowFile = "";
        emit reflashRate(precent);
        //隐藏当前窗体
        this->hide();
        //显示当前窗体的父亲
        this->parentWidget()->raise();
        this->parentWidget()->show();
}

void FuncWindow::getRate()
{
    mPro->write("get_time_pos \n");
}

void FuncWindow::readData()
{
    QString msg = mPro->readAll();

    if(msg.indexOf("ANS_TIME_POSITION") > -1)
    {
        msg.remove(0, msg.lastIndexOf("=")+1);
        preTime = msg.toDouble();       
        int aim = (int)((preTime/allTime) *100);
        //通过计算时间去改变当前百分比
        precent = aim;
        emit reflashRate(precent);
        qDebug() << "allTime : " << allTime << "preTime : " << preTime << "precent : " << precent;

    }
    else if(msg.indexOf("ANS_LENGTH") > -1)
    {
        //获取总的时间
        msg.remove(0, msg.lastIndexOf("=")+1);
        QStringList temp = msg.split("\n");
        allTime = temp.at(0).toDouble();
        qDebug() << "allTime : " << allTime;
    }
}

//读取文件路径
void FuncWindow::readFileName()
{
    if(variety == 1)
    {
        if(!fileList.isEmpty())
            fileList.clear();

        QDir vidFile(FilePath);
        //将所有需要的文件读取出来
        fileList = vidFile.entryList().filter(".mp4");

        refreshList();
    }
    else if(variety == 2)
    {
        if(!fileList.isEmpty())
            fileList.clear();

        QDir vidFile(FilePath);
        //将所有需要的文件读取出来
        fileList = vidFile.entryList().filter(".mp3");
        refreshList();
    }
}

//更新列表
void FuncWindow::refreshList()
{
    ui->listWidget->clear();

    for(int i = 0; i < fileList.size(); i++)
    {
        if(variety == 1)
        {
            QString iconPath = fileList.at(i);

            QIcon thisIcon(FilePath + iconPath.remove("mp4") + "jpg");
            qDebug() << "shipingtubiao" << FilePath + iconPath.remove("mp4") + "jpg";
            QListWidgetItem *newfile = new QListWidgetItem(thisIcon, fileList.at(i));
            ui->listWidget->addItem(newfile);
        }
        else
            ui->listWidget->addItem(fileList.at(i));
    }

}

//停止定时器播放
void FuncWindow::stopTimer()
{
    timer->stop();
}

//跳转到指定进度
void FuncWindow::jumpAim(int pre)
{
    //更新滑动的位置
    QString command = "seek " + QString::number(pre) + " 1 \n";

    mPro->write(command.toUtf8());

//    getRate();

    timer->start(500);

}

//播放结束的回调函数
void FuncWindow::overHandle()
{
    qDebug() << "now over";
    if(precent > 90)
    {
        playTime = 0;
        nextPlay();
    }
}

void FuncWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //确保当前是空闲状态
    if(playTime != 0)
    {
        mPro->kill();
        mPro->waitForFinished();
    }
    playTime = 1;
    qDebug() << item->text();
    if(variety == 1)
    {
        nowFile = item->text();
        QString command = "mplayer  -slave  -quiet  -zoom  -x  600  -y  400 ";
        mPro->start(command + FilePath + nowFile);
        QString setVol = "volume " + QString::number(volume) + " 1\n";
        mPro->write(setVol.toUtf8());

        qDebug() << command + FilePath + nowFile;
        qDebug() << setVol;
    }
    else if(variety == 2)
    {
        nowFile = item->text();
        readyData();
        QString command = "mplayer -slave -quiet " ;
        mPro->start(command + FilePath + nowFile);
        QString setVol = "volume " + QString::number(volume) + " 1\n";
        mPro->write(setVol.toUtf8());
        qDebug() << command + FilePath + nowFile;
        qDebug() << setVol;

    }
    //获取总长
    mPro->write("get_time_length \n");


    timer->start(500);
}

void FuncWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //确保当前是空闲状态
    if(playTime != 0)
    {
        mPro->kill();
        mPro->waitForFinished();
    }
    playTime = 1;
    qDebug() << item->text();
    if(variety == 1)
    {
        nowFile = item->text();
        QString command = "mplayer  -slave  -quiet  -zoom  -x  600  -y  400 ";
        mPro->start(command + FilePath + nowFile);
        QString setVol = "volume " + QString::number(volume) + " 1\n";
        mPro->write(setVol.toUtf8());

        qDebug() << command + FilePath + nowFile;
        qDebug() << setVol;
    }
    else if(variety == 2)
    {
        nowFile = item->text();
        readyData();
        QString command = "mplayer -slave -quiet " ;
        mPro->start(command + FilePath + nowFile);
        QString setVol = "volume " + QString::number(volume) + " 1\n";
        mPro->write(setVol.toUtf8());
        qDebug() << command + FilePath + nowFile;
        qDebug() << setVol;

    }
    //获取总长
    mPro->write("get_time_length \n");

    timer->start(500);
}
