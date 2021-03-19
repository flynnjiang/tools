#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include <QVector>
#include <QFile>
#include <stdio.h>
#include <QDataStream>
#include <QByteArray>
#include "tycan.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("TyCan - ") + __DATE__ + " By Flynn");

    ui->tableWidget_Packets->horizontalHeader()->resizeSection(0, 90);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(1, 70);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(2, 45);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(3, 45);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(4, 40);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(5, 45);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(6, 80);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(7, 35);
    ui->tableWidget_Packets->horizontalHeader()->resizeSection(8, 160);

    m_readerThread.setCan(&m_tyCan);
    connect(&m_readerThread, SIGNAL(sendRawTeleData(QVariant)), this, SLOT(recvTeleData(QVariant)));
}

MainWindow::~MainWindow()
{
    on_pushButton_CloseDev_clicked();

    delete ui;
}


void MainWindow::showTips(const QString &tips)
{
    ui->statusBar->showMessage(tips);
}


void MainWindow::recvTeleData(QVariant var)
{
    CAN_OBJ *frame = NULL;
    struct CanMsg msg = var.value<struct CanMsg>();
    int curr = 0;
    QTableWidgetItem *item = NULL;
    ty_can_id_t id;
    int i = 0, j = 0;
    char buf[128];
    int len = 0;

    qDebug("msg.num = %d", msg.num);

    int fltrMode = ui->comboBox_FltrMode->currentIndex();
    int fltrCanId = ui->spinBox_FltrCanId->value();


    for (i = 0; i < msg.num; i++) {

        qDebug("add id = %d", i);

        frame = &msg.objs[i];
        id.value = frame->ID;

        /*********************************************
         * 过滤
         *********************************************/
        if (1 == fltrMode) { // 发送或接收
            if (fltrCanId != id.ext.src && fltrCanId != id.ext.dest) {
                continue;
            }
        } else if (2 == fltrMode) { // 发送
            if (fltrCanId != id.ext.src) {
                continue;
            }
        } else if (3 == fltrMode) { // 接收
            if (fltrCanId != id.ext.dest) {
                continue;
            }
        }


        /*********************************************
         * 显示
         *********************************************/
        curr = ui->tableWidget_Packets->rowCount();
        ui->tableWidget_Packets->insertRow(curr);


        snprintf(buf, sizeof(buf), "%d.%03d.%03d", frame->TimeStamp / 1000000,
                                                   frame->TimeStamp / 1000 % 1000,
                                                   frame->TimeStamp % 1000);
        item = new QTableWidgetItem(buf);
        if (item) {
            item->setTextAlignment(Qt::AlignRight);
            ui->tableWidget_Packets->setItem(curr, 0, item);
        }

        snprintf(buf, sizeof(buf), "%08X", frame->ID);
        item = new QTableWidgetItem(buf);
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 1, item);
        }


        item = new QTableWidgetItem(QString::number(id.ext.src));
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 2, item);
        }

        item = new QTableWidgetItem(QString::number(id.ext.dest));
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 3, item);
        }

        item = new QTableWidgetItem(QString::number(id.ext.seq));
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 4, item);
        }


        switch (id.ext.info) {
        case 0: snprintf(buf, sizeof(buf), "%s", "RAW"); break;
        case 1: snprintf(buf, sizeof(buf), "%s", "CSP"); break;
        default: snprintf(buf, sizeof(buf), "%d", id.ext.info); break;
        }
        item = new QTableWidgetItem(buf);
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 5, item);
        }

        switch (id.ext.type) {
        case 0: snprintf(buf, sizeof(buf), "%s", "恢复帧"); break;
        case 1: snprintf(buf, sizeof(buf), "%s", "单帧"); break;
        case 2: snprintf(buf, sizeof(buf), "%s", "复合起始帧"); break;
        case 3: snprintf(buf, sizeof(buf), "%s", "复合中间帧"); break;
        case 4: snprintf(buf, sizeof(buf), "%s", "时间同步帧"); break;
        default: snprintf(buf, sizeof(buf), "%d", id.ext.type); break;
        }

        item = new QTableWidgetItem(buf);
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 6, item);
        }


        item = new QTableWidgetItem(QString::number(frame->DataLen));
        if (item) {
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget_Packets->setItem(curr, 7, item);
        }


        for (j = 0, len = 0; j < frame->DataLen; j++) {
            len += snprintf(buf + len, sizeof(buf) - len, "%02X ", frame->Data[j]);
        }
        item = new QTableWidgetItem(buf);
        if (item) {
            ui->tableWidget_Packets->setItem(curr, 8, item);
        }
    }

    if (ui->checkBox_AutoScroll->isChecked()) {
        ui->tableWidget_Packets->scrollToBottom();
    }
}


QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                ret.clear();
                return ret;
            }
        }
    }
    return ret;
}


void MainWindow::on_pushButton_OpenDev_clicked()
{
    int i = 0;
    bool bRet = false;
    char path[256] = {0};
    uint8_t devType;

    switch (ui->comboBox_ChId->currentIndex()) {
    case 0:
        devType = 4;
        break;
    case 1:
        devType = 3;
        break;
    default:
        return;
        break;
    }

    bRet = m_tyCan.openDev(devType, ui->comboBox_DevId->currentIndex(), ui->comboBox_ChId->currentIndex());
    if (bRet) {
        ui->pushButton_OpenDev->setEnabled(false);
        ui->pushButton_CloseDev->setEnabled(true);
        ui->pushButton_Send->setEnabled(true);
        ui->comboBox_DevId->setEnabled(false);
        ui->comboBox_DevType->setEnabled(false);
        ui->comboBox_ChId->setEnabled(false);

        m_readerThread.start();

        showTips("打开CAN适配器成功！");

        return;
    }

    showTips("打开CAN适配器失败！");
}


void MainWindow::on_pushButton_CloseDev_clicked()
{
    int i = 0;

    if (m_readerThread.isRunning()) {
        showTips("等待采集线程退出……");
        m_readerThread.stop();
        m_readerThread.wait();
    }

    if (! m_tyCan.closeDev()) {
        showTips("关闭CAN适配器失败！");
        return;
    }

    ui->pushButton_OpenDev->setEnabled(true);
    ui->pushButton_CloseDev->setEnabled(false);
    ui->pushButton_Send->setEnabled(false);
    ui->comboBox_DevId->setEnabled(true);
    ui->comboBox_DevType->setEnabled(true);
    ui->comboBox_ChId->setEnabled(true);

    showTips("关闭CAN适配器成功！");
}



void MainWindow::on_pushButton_Clear_clicked()
{
    ui->tableWidget_Packets->clearContents();
    ui->tableWidget_Packets->setRowCount(0);
}

void MainWindow::on_pushButton_Send_clicked()
{
    QByteArray dataBytes;
    struct ty_can_frame frame;
    int i = 0;

    dataBytes = HexStringToByteArray(ui->plainTextEdit_SendData->toPlainText());
    if (dataBytes.size() <= 0) {
        return;
    }

    qDebug("dataBytes: num = %d", dataBytes.size());

    memset(&frame, 0, sizeof(frame));
    frame.can_id.ext.src = ui->spinBox_SendSrc->value();
    frame.can_id.ext.dest = ui->spinBox_SendDest->value();
    frame.data_type = dataBytes[0];
    frame.data_id = dataBytes[1];
    for (i = 2; i < dataBytes.size(); i++) {
        frame.data[i - 2] = dataBytes[i];
    }
    frame.data_len = dataBytes.size() - 2;

    m_tyCan.sendFrame(&frame);
}

void MainWindow::on_comboBox_FltrMode_currentIndexChanged(int index)
{
    ui->spinBox_FltrCanId->setEnabled(0 != index);
}
