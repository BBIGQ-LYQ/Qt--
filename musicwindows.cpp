#include "musicwindows.h"
#include <QListWidget>
#include <QDebug>

musicWindows::musicWindows(QWidget *parent):FuncWindow(parent)
{
    variety = 2;
    listWidget = new QListWidget(this);

    listWidget->setGeometry(QRect(50, 50, 500, 350));
    listWidget->setMinimumSize(QSize(500, 350));
    listWidget->setMaximumSize(QSize(500, 350));

    Title = new QLabel(this);
    Title->setGeometry(100, 10, 400, 30);
    Title->setMinimumSize(QSize(400, 30));
    Title->setMaximumSize(QSize(400, 30));

    QFont ft;
    ft.setPointSize(16);
    Title->setFont(ft);


}

musicWindows::~musicWindows()
{
    delete listWidget;
    delete Title;
}

void musicWindows::out()
{
    mPro->kill();
    mPro->waitForFinished();
//        //退出播放器
//        mPro->write("quit 0\n");
    //停止定时器
    timer->stop();

    //所有参数复位
    listWidget->clear();
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

void musicWindows::getLrc()
{
    if(!lrcTimeList.isEmpty())
    {
        lrcTimeList.clear();
        listWidget->clear();
    }
    QStringList temp;
    //临时存储时间
    QStringList timeTemp;
    QStringList parseTime;
    QString name = nowFile;
    QString lrcPath = FilePath + name.remove("mp3") + "lrc";

    QFile lrcFile(lrcPath);
    if (!lrcFile.open(QIODevice::ReadOnly | QIODevice::Text))
              return;
    while (!lrcFile.atEnd()) {
                QString line = lrcFile.readLine();
                temp = line.split("]");
                if(temp.at(1).size() > 1)
                    listWidget->addItem(temp.at(1));
                QStringList buff = temp.at(0).split("[");
                timeTemp.append(buff.at(1));
          }

    for(int i = 5; i < timeTemp.size(); i++)
    {
        parseTime = timeTemp.at(i).split(":");
        QString min = parseTime.at(0);
        QString sec = parseTime.at(1);

        double group = min.toDouble() * 60 + sec.toDouble();


        lrcTimeList.append(QString::number(group));

    }
}

void musicWindows::showLrc()
{
    int now = 0;
    double thisTime = preTime;
    qDebug() << "showLrc " << thisTime;
    for(int i = 0; i < lrcTimeList.size()-1; i ++)
    {
        if(thisTime >= lrcTimeList.at(i).toDouble() && thisTime <= lrcTimeList.at(i+1).toDouble())
        {
            now = i;
            listWidget->setCurrentRow(now);
            listWidget->scrollToItem(listWidget->currentItem(), QAbstractItemView::PositionAtCenter);
            break;
        }
    }
}

void musicWindows::readData()
{
    QString msg = mPro->readAll();
    qDebug() << "music process " << msg ;
    if(msg.indexOf("ANS_TIME_POSITION") > -1)
    {
        msg.remove(0, msg.lastIndexOf("=")+1);
        QString thisMsg = msg;
        preTime = thisMsg.toDouble();
         //通过计算时间去改变当前百分比
        precent = (int)(preTime/allTime *100);
        emit reflashRate(precent);
        //刷字幕
        showLrc();
        qDebug() << "allTime : " << allTime << "preTime : " << preTime << "precent : " << precent;
    }
    else if(msg.indexOf("ANS_LENGTH") > -1)
    {
        //获取总的时间
        msg.remove(0, msg.lastIndexOf("=")+1);
        QStringList temp = msg.split("\n");
        allTime = temp.at(0).toDouble();
    }
}

void musicWindows::readyData()
{
    getLrc();
    Title->setText(nowFile);
}

