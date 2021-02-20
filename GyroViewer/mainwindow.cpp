#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include <QVector>
#include <QFile>
#include <stdio.h>
#include <QDataStream>

#include "qcustomplot.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i = 0, j = 0;
    QCPGraph *graph = NULL;

    ui->setupUi(this);

    this->setWindowTitle(QString("GyroViewer ") + QString(__DATE__));

    m_readerThread.setCan(&m_tyCan);
    m_readerThread.setAcqFreq(ui->spinBox_AcqFreq->value());
    connect(&m_readerThread, SIGNAL(sendRawTeleData(QVariant)), this, SLOT(recvRawTeleData(QVariant)));

    clearData();

    for (i = 0; i < 2; i++) {

        m_magFile[i] = NULL;
        m_gyrFile[i] = NULL;
        m_accFile[i] = NULL;

        for (j = 0; j < 3; j++) {

            /* GYRO */
            m_gyrPlot[i][j] = new QCustomPlot(this);
            m_gyrPlot[i][j]->xAxis->setLabel("计数");
            switch (j) {
                case 0: m_gyrPlot[i][j]->yAxis->setLabel(QString("陀螺仪") + QString::number(i) + "-X轴（°/s）"); break;
                case 1: m_gyrPlot[i][j]->yAxis->setLabel(QString("陀螺仪") + QString::number(i) + "-Y轴（°/s）"); break;
                case 2: m_gyrPlot[i][j]->yAxis->setLabel(QString("陀螺仪") + QString::number(i) + "-Z轴（°/s）"); break;
            }
            m_gyrPlot[i][j]->xAxis->setRange(0, DATA_MAX_NUM);
            m_gyrPlot[i][j]->yAxis->setRange(-100, 100);
            graph = m_gyrPlot[i][j]->addGraph();
            graph->setPen(QPen(QColor(0 == i ? "blue" : "green")));
            ui->gridLayout_gyro->addWidget(m_gyrPlot[i][j], i, j);



            /* ACCL */
            m_accPlot[i][j] = new QCustomPlot(this);
            m_accPlot[i][j]->xAxis->setLabel("计数");
            switch (j) {
                case 0: m_accPlot[i][j]->yAxis->setLabel(QString("加速度计") + QString::number(i) + "-X轴（mg）"); break;
                case 1: m_accPlot[i][j]->yAxis->setLabel(QString("加速度计") + QString::number(i) + "-Y轴（mg）"); break;
                case 2: m_accPlot[i][j]->yAxis->setLabel(QString("加速度计") + QString::number(i) + "-Z轴（mg）"); break;
            }
            m_accPlot[i][j]->xAxis->setRange(0, DATA_MAX_NUM);
            m_accPlot[i][j]->yAxis->setRange(-100, 100);
            graph = m_accPlot[i][j]->addGraph();
            graph->setPen(QPen(QColor(0 == i ? "blue" : "green")));
            ui->gridLayout_accl->addWidget(m_accPlot[i][j], i, j);


            /* MAG */
            m_magPlot[i][j] = new QCustomPlot(this);
            m_magPlot[i][j]->xAxis->setLabel("计数");
            switch (j) {
                case 0: m_magPlot[i][j]->yAxis->setLabel(QString("磁强计") + QString::number(i) + "-X轴（mG）"); break;
                case 1: m_magPlot[i][j]->yAxis->setLabel(QString("磁强计") + QString::number(i) + "-Y轴（mG）"); break;
                case 2: m_magPlot[i][j]->yAxis->setLabel(QString("磁强计") + QString::number(i) + "-Z轴（mG）"); break;
            }
            m_magPlot[i][j]->xAxis->setRange(0, DATA_MAX_NUM);
            m_magPlot[i][j]->yAxis->setRange(-100, 100);
            graph = m_magPlot[i][j]->addGraph();
            graph->setPen(QPen(QColor(0 == i ? "blue" : "green")));
            ui->gridLayout_mag->addWidget(m_magPlot[i][j], i, j);
        }
    }
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


void MainWindow::clearData(void)
{
    int i = 0, j = 0;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            m_magData[i][j].clear();
            m_gyrData[i][j].clear();
            m_accData[i][j].clear();
        }
    }
}


void MainWindow::recvRawTeleData(QVariant var)
{
    int i = 0, j = 0;
    static int cnt = 0;
    double value = 0.0;
    struct ty_gyr_tele_data data = var.value<struct ty_gyr_tele_data>();
    std::string now = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString();

    cnt++;

    for (i = 0; i < 2; i++) {

        fprintf(m_magFile[i], "\n%s,%.2lf", now.c_str(), (double)qFromBigEndian(data.mag[i].temp) / 100.0);
        fprintf(m_gyrFile[i], "\n%s,%.2lf", now.c_str(), (double)qFromBigEndian(data.gyr[i].temp) / 100.0);
        fprintf(m_accFile[i], "\n%s,%.2lf", now.c_str(), (double)qFromBigEndian(data.gyr[i].temp) / 100.0);

        for (j = 0; j < 3; j++) {
            value = (double)qFromBigEndian(data.mag[i].data[j]) * 0.0625; // 0.0625mG/LSB
            fprintf(m_magFile[i], ",%lf", value);
            m_magData[i][j].addData(cnt, value);
            m_magPlot[i][j]->graph(0)->setData(m_magData[i][j].m_key, m_magData[i][j].m_val);
            m_magPlot[i][j]->xAxis->setLabel(QString("当前:") + QString::number(value)
                                           + QString(" | 原始:") + QString::number(qFromBigEndian(data.mag[i].data[j]))
                                           + QString(" | 振幅:") + QString::number(m_magData[i][j].m_val_max - m_magData[i][j].m_val_min));
            m_magPlot[i][j]->xAxis->setRange(m_magData[i][j].m_key_min, MAX(DATA_MAX_NUM, m_magData[i][j].m_key_max));
            m_magPlot[i][j]->yAxis->setRange((int32_t)(m_magData[i][j].m_val_min)  / 10 * 10 - 20,
                                             (int32_t)(m_magData[i][j].m_val_max)  / 10 * 10 + 20);
            m_magPlot[i][j]->replot();


            value = (double)qFromBigEndian(data.gyr[i].gyr[j]) * 0.1 / 65536.0; // 0.1/65536°/sec/LSB
            fprintf(m_gyrFile[i], ",%lf", value);
            m_gyrData[i][j].addData(cnt, value);
            m_gyrPlot[i][j]->graph(0)->setData(m_gyrData[i][j].m_key, m_gyrData[i][j].m_val);
            m_gyrPlot[i][j]->xAxis->setLabel(QString("当前:") + QString::number(value)
                                           + QString(" | 原始:") + QString::number(qFromBigEndian(data.gyr[i].gyr[j]))
                                           + QString(" | 振幅:") + QString::number(m_gyrData[i][j].m_val_max - m_gyrData[i][j].m_val_min));
            m_gyrPlot[i][j]->xAxis->setRange(m_gyrData[i][j].m_key_min, MAX(DATA_MAX_NUM, m_gyrData[i][j].m_key_max));
            m_gyrPlot[i][j]->yAxis->setRange((int32_t)(m_gyrData[i][j].m_val_min) - 1,
                                             (int32_t)(m_gyrData[i][j].m_val_max) + 1);
            m_gyrPlot[i][j]->replot();



            value = (double)qFromBigEndian(data.gyr[i].acc[j]) * 1.25 / 65536.0; // 1.25/65536 mg/LSB
            fprintf(m_accFile[i], ",%lf", value);
            m_accData[i][j].addData(cnt, value);
            m_accPlot[i][j]->xAxis->setLabel(QString("当前:") + QString::number(value)
                                           + QString(" | 原始:") + QString::number(qFromBigEndian(data.gyr[i].acc[j]))
                                           + QString(" | 振幅:") + QString::number(m_accData[i][j].m_val_max - m_accData[i][j].m_val_min));
            m_accPlot[i][j]->xAxis->setRange(m_accData[i][j].m_key_min, MAX(DATA_MAX_NUM, m_accData[i][j].m_key_max));
            m_accPlot[i][j]->yAxis->setRange((int32_t)(m_accData[i][j].m_val_min)  / 10 * 10 - 20,
                                             (int32_t)(m_accData[i][j].m_val_max)  / 10 * 10 + 20);
            m_accPlot[i][j]->graph(0)->setData(m_accData[i][j].m_key, m_accData[i][j].m_val);
            m_accPlot[i][j]->replot();
        }

        fflush(m_magFile[i]);
        fflush(m_gyrFile[i]);
        fflush(m_accFile[i]);

    }

    if (data.mag_stat[0]) {
        ui->label_Mag0Valid->setText("有效");
        ui->label_Mag0Valid->setStyleSheet("background-color: green;");
    } else {
        ui->label_Mag0Valid->setText("无效");
        ui->label_Mag0Valid->setStyleSheet("background-color: red;");
    }

    if (data.mag_stat[1]) {
        ui->label_Mag1Valid->setText("有效");
        ui->label_Mag1Valid->setStyleSheet("background-color: green;");
    } else {
        ui->label_Mag1Valid->setText("无效");
        ui->label_Mag1Valid->setStyleSheet("background-color: red;");
    }

    if (data.gyr_stat[0]) {
        ui->label_Gyr0Valid->setText("有效");
        ui->label_Gyr0Valid->setStyleSheet("background-color: green;");
    } else {
        ui->label_Gyr0Valid->setText("无效");
        ui->label_Gyr0Valid->setStyleSheet("background-color: red;");
    }

    if (data.gyr_stat[1]) {
        ui->label_Gyr1Valid->setText("有效");
        ui->label_Gyr1Valid->setStyleSheet("background-color: green;");
    } else {
        ui->label_Gyr1Valid->setText("无效");
        ui->label_Gyr1Valid->setStyleSheet("background-color: red;");
    }

    ui->lcdNumber_Gyr0->display((double)qFromBigEndian(data.gyr[0].temp) / 100.0);
    ui->lcdNumber_Gyr1->display((double)qFromBigEndian(data.gyr[1].temp) / 100.0);
    ui->lcdNumber_Mag0->display((double)qFromBigEndian(data.mag[0].temp) / 100.0);
    ui->lcdNumber_Mag1->display((double)qFromBigEndian(data.mag[1].temp) / 100.0);
    ui->lcdNumber_Time->display((double)qFromBigEndian(data.time) / 1000.0);
}


void MainWindow::on_pushButton_OpenDev_clicked()
{
    int i = 0;
    bool bRet = false;
    char path[256] = {0};
    uint8_t devType;
    std::string now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss").toStdString();

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
        ui->pushButton_ResetDev->setEnabled(true);
        ui->comboBox_DevId->setEnabled(false);
        ui->comboBox_DevType->setEnabled(false);
        ui->comboBox_ChId->setEnabled(false);

        for (i = 0; i < 2; i++) {
            snprintf(path, sizeof(path), "d:\\%s_MAG%d.csv", now.c_str(), i);
            m_magFile[i] = fopen(path, "w");

            snprintf(path, sizeof(path), "d:\\%s_GYR%d.csv", now.c_str(), i);
            m_gyrFile[i] = fopen(path, "w");

            snprintf(path, sizeof(path), "d:\\%s_ACC%d.csv", now.c_str(), i);
            m_accFile[i] = fopen(path, "w");
        }

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

    for (i = 0; i < 2; i++) {
        if (m_magFile[i]) {
            fclose(m_magFile[i]);
            m_magFile[i] = NULL;
        }

        if (m_gyrFile[i]) {
            fclose(m_gyrFile[i]);
            m_gyrFile[i] = NULL;
        }

        if (m_accFile[i]) {
            fclose(m_accFile[i]);
            m_accFile[i] = NULL;
        }
    }

    if (! m_tyCan.closeDev()) {
        showTips("关闭CAN适配器失败！");
        return;
    }

    ui->pushButton_OpenDev->setEnabled(true);
    ui->pushButton_CloseDev->setEnabled(false);
    ui->pushButton_ResetDev->setEnabled(false);
    ui->comboBox_DevId->setEnabled(true);
    ui->comboBox_DevType->setEnabled(true);
    ui->comboBox_ChId->setEnabled(true);

    showTips("关闭CAN适配器成功！");
}

void MainWindow::on_pushButton_ResetDev_clicked()
{
    if (! m_tyCan.resetDev()) {
        showTips("复位CAN失败！");
        return;
    }

    showTips("复位CAN成功！");
}

void MainWindow::on_spinBox_AcqFreq_valueChanged(int arg1)
{
    m_readerThread.setAcqFreq(arg1);
}

void MainWindow::on_comboBox_WorkMode_currentIndexChanged(int index)
{
    m_readerThread.setAcqEn(0 == index);
    ui->spinBox_AcqFreq->setEnabled(0 == index);
}
