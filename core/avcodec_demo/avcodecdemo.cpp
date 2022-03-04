#include "avcodecdemo.h"
#include "ui_avcodecdemo.h"

AVCodecDemo::AVCodecDemo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AVCodecDemo)
{   
    ui->setupUi(this);
}

AVCodecDemo::~AVCodecDemo()
{
    delete ui;
}

void AVCodecDemo::on_pushButton_clicked()
{
    avcodec.test();
}
