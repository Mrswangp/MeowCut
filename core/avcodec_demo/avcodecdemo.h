#ifndef AVCODECDEMO_H
#define AVCODECDEMO_H

#include "avcodec/avcodec.h"

#include <QMainWindow>

namespace Ui {
class AVCodecDemo;
}

class AVCodecDemo : public QMainWindow
{
    Q_OBJECT

public:
    explicit AVCodecDemo(QWidget *parent = nullptr);
    ~AVCodecDemo();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AVCodecDemo *ui;
    AVCodec avcodec;
};

#endif // AVCODECDEMO_H
