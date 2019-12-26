#include "toolswindow.h"
#include "ui_toolswindow.h"

ToolsWindow::ToolsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolsWindow)
{
    ui->setupUi(this);
    connect(this->parentWidget(), SIGNAL(reflashRate(int)), this, SLOT(reflashRateRealTime(int)));
}

ToolsWindow::~ToolsWindow()
{
    delete ui;
}

void ToolsWindow::reflashRateRealTime(int pos)
{

    ui->precent_seek->setValue(pos);
}

void ToolsWindow::on_but_out_clicked()
{
    emit OUT();
}

void ToolsWindow::on_but_rev_clicked()
{
    emit RETURNB();
}

void ToolsWindow::on_but_frev_clicked()
{
    emit PREVSIG();
}

void ToolsWindow::on_but_play_clicked()
{
    emit PLAYSIG();
}

void ToolsWindow::on_but_next_clicked()
{
    emit NEXTSIG();
}

void ToolsWindow::on_but_adv_clicked()
{
    emit MOVEPRE();
}

void ToolsWindow::on_but_volup_clicked()
{
    emit VOLADD();
}

void ToolsWindow::on_but_voldown_clicked()
{
    emit VOLSUB();
}


//拖动进度条
void ToolsWindow::on_precent_seek_sliderPressed()
{
    emit STOPTIMER();
}

void ToolsWindow::on_precent_seek_sliderReleased()
{
    int aimPrecent;
    aimPrecent = ui->precent_seek->value();
    qDebug() << "SKIP :" << aimPrecent;
    emit SKIP(aimPrecent);
}
