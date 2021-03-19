#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include <QVector>
#include <QFile>
#include <stdio.h>
#include <QDataStream>
#include <math.h>

#include "qcustomplot.h"


//全局变量初始化，用于最小二乘计算磁强计校正参数
static double g_xk[12] = { 1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000 };
static double g_pk[144] = { 1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000 };

/**< 地球自转角速度 */
static const double g_w_ie = 7.292e-5;
const double PI = 3.14159265358979;
const double DEG2PI = 0.0174532925199433;
const double PI2DEG = 57.295779513082323;

/* 10000 高斯 = 1 特思拉 */
#define MG2T(g) ((double)(g) / 1000.0 / 10000.0)
#define T2MG(t) ((double)(t) * 10000.0 * 1000.0)

/* 1 RAW = 0.0625 mG, 1 T = 1.0e4 G */
#define MAG_RAW2T(r)    ((double)(r) * 6.25e-9)
#define MAG_T2RAW(t)    ((double)(r) * 1.6e8)

/* 1 RAW = 0.1 / 65536 deg/s, 1 deg/s = PI / 180 rad/s */
#define GYR_RAW2RAD(r)  ((double)(r) * 0.000000026631611)
#define GYR_RAD2RAW(r)  ((double)(r) * 37549361.921815394494911)


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i = 0, j = 0;
    QCPGraph *graph = NULL;

    ui->setupUi(this);

    this->setWindowTitle(QString("GyroViewer - ") + __DATE__ );

    m_readerThread.setCan(&m_tyCan);
    m_readerThread.setAcqFreq(ui->spinBox_AcqFreq->value());
    connect(&m_readerThread, SIGNAL(sendRawTeleData(QVariant)), this, SLOT(recvTeleData(QVariant)));

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

    /****************************************************************
     * 校准相关的控件
     ****************************************************************/

    m_caliMagParamSpinBox[0][0] = ui->doubleSpinBox_m0_m11;
    m_caliMagParamSpinBox[0][1] = ui->doubleSpinBox_m0_m12;
    m_caliMagParamSpinBox[0][2] = ui->doubleSpinBox_m0_m13;
    m_caliMagParamSpinBox[0][3] = ui->doubleSpinBox_m0_m21;
    m_caliMagParamSpinBox[0][4] = ui->doubleSpinBox_m0_m22;
    m_caliMagParamSpinBox[0][5] = ui->doubleSpinBox_m0_m23;
    m_caliMagParamSpinBox[0][6] = ui->doubleSpinBox_m0_m31;
    m_caliMagParamSpinBox[0][7] = ui->doubleSpinBox_m0_m32;
    m_caliMagParamSpinBox[0][8] = ui->doubleSpinBox_m0_m33;
    m_caliMagParamSpinBox[0][9] = ui->doubleSpinBox_m0_v1;
    m_caliMagParamSpinBox[0][10] = ui->doubleSpinBox_m0_v2;
    m_caliMagParamSpinBox[0][11] = ui->doubleSpinBox_m0_v3;

    m_caliMagParamSpinBox[1][0] = ui->doubleSpinBox_m1_m11;
    m_caliMagParamSpinBox[1][1] = ui->doubleSpinBox_m1_m12;
    m_caliMagParamSpinBox[1][2] = ui->doubleSpinBox_m1_m13;
    m_caliMagParamSpinBox[1][3] = ui->doubleSpinBox_m1_m21;
    m_caliMagParamSpinBox[1][4] = ui->doubleSpinBox_m1_m22;
    m_caliMagParamSpinBox[1][5] = ui->doubleSpinBox_m1_m23;
    m_caliMagParamSpinBox[1][6] = ui->doubleSpinBox_m1_m31;
    m_caliMagParamSpinBox[1][7] = ui->doubleSpinBox_m1_m32;
    m_caliMagParamSpinBox[1][8] = ui->doubleSpinBox_m1_m33;
    m_caliMagParamSpinBox[1][9] = ui->doubleSpinBox_m1_v1;
    m_caliMagParamSpinBox[1][10] = ui->doubleSpinBox_m1_v2;
    m_caliMagParamSpinBox[1][11] = ui->doubleSpinBox_m1_v3;

    m_caliGyrDegProgressBar[0] = ui->progressBar_magX;
    m_caliGyrDegProgressBar[1] = ui->progressBar_magY;
    m_caliGyrDegProgressBar[2] = ui->progressBar_magZ;

    m_caliState = TY_CALI_STATE_OFF;

    CaliInit();
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


void MainWindow::recvTeleData(QVariant var)
{
    int i = 0, j = 0;
    int base = 10;
    static int cnt = 0;
    double value = 0.0;
    struct ty_gyr_tele_data data = var.value<struct ty_gyr_tele_data>();
    struct TyGyrMagData gyrMagData;
    std::string now = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString();

    ConvertTeleData(&data, &gyrMagData);
    CaliRun(&gyrMagData);

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

    ui->label_SysTime->setText(QString::number((double)((uint32_t)qFromBigEndian(data.time)) / 1000.0, 'f', 3));
    ui->label_CanOkCnt->setText(QString::number(data.can_ok_cnt));
    ui->label_CanErrCnt->setText(QString::number(data.can_err_cnt));
    ui->label_TeleCnt->setText(QString::number(data.req_cnt));
    ui->label_LastestCmd->setText(QString::number(data.lastest_cmd));

    base = 10;
    switch (data.param_id) {
        case 1:   ui->label_ParamId->setText("陀螺仪1原始数据"); break;
        case 2:   ui->label_ParamId->setText("陀螺仪2原始数据"); break;
        case 4:   ui->label_ParamId->setText("加速度计1原始数据"); break;
        case 5:   ui->label_ParamId->setText("加速度计2原始数据"); break;
        case 51:  ui->label_ParamId->setText("磁强计1SET数据"); break;
        case 52:  ui->label_ParamId->setText("磁强计1RESET数据"); break;
        case 53:  ui->label_ParamId->setText("磁强计1原始数据"); break;
        case 54:  ui->label_ParamId->setText("磁强计2SET数据"); break;
        case 55:  ui->label_ParamId->setText("磁强计2RESET数据"); break;
        case 56:  ui->label_ParamId->setText("磁强计2原始数据"); break;
        case 60:  ui->label_ParamId->setText("磁强计1零偏校准"); break;
        case 61:  ui->label_ParamId->setText("磁强计2零偏校准"); break;
        case 62:  ui->label_ParamId->setText("磁强计1比例校准"); break;
        case 63:  ui->label_ParamId->setText("磁强计2比例校准"); break;
        case 101: ui->label_ParamId->setText("系统启动信息"); base = 16; break;
        case 102: ui->label_ParamId->setText("内存信息"); base = 16; break;
        default:  ui->label_ParamId->setText("未知标识"); break;
    }

    ui->label_Param1->setText(QString::number((int32_t)qFromBigEndian(data.param32[0]), base));
    ui->label_Param2->setText(QString::number((int32_t)qFromBigEndian(data.param32[1]), base));
    ui->label_Param3->setText(QString::number((int32_t)qFromBigEndian(data.param32[2]), base));
    ui->label_Param4->setText(QString::number((int16_t)qFromBigEndian(data.param16), base));

}


bool MainWindow::ConvertTeleData(const struct ty_gyr_tele_data *tele, struct TyGyrMagData *data)
{
    int i = 0, j = 0;

    data->time = qFromBigEndian(tele->time);

    for (i = 0; i < 2; i++) {
        data->gyr[i].valid = tele->gyr_stat[i] ? true : false;
        data->mag[i].valid = tele->mag_stat[i] ? true : false;

        data->gyr[i].temp = (double)qFromBigEndian(tele->gyr[i].temp) / 100.0;
        data->mag[i].temp = (double)qFromBigEndian(tele->mag[i].temp) / 100.0;

        for (j = 0; j < 3; j++) {
            data->gyr[i].gyr[j] = (double)qFromBigEndian(tele->gyr[i].gyr[j]) * 0.1 / 65536.0;
            data->gyr[i].acc[j] = (double)qFromBigEndian(tele->gyr[i].acc[j]) * 1.25 / 65536.0;
            data->mag[i].data[j] = (double)qFromBigEndian(tele->mag[i].data[j]) * 0.0625;
        }
    }

    return true;
}



bool MainWindow::CaliInit(void)
{
    int i = 0, j = 0;

    /*  初始化 */
    m_caliGyrMagData.clear();

    for (i = 0; i < 2; i++) {

        memcpy(&m_caliMagCaliXk[i], g_xk, sizeof(g_xk));
        memcpy(&m_caliMagCaliPk[i], g_pk, sizeof(g_pk));

        for (j = 0; j < 12; j++) {
            m_caliMagParamSpinBox[i][j]->setValue(m_caliMagCaliXk[i][j]);
        }
    }

    for (i = 0; i < 3; i++) {
        m_caliGyrdegCur[i] = 0.0;
        m_caliGyrdegMax[i] = 0.0;
        m_caliGyrdegMin[i] = 0.0;
        m_caliGyrDegProgressBar[i]->setValue(0);
    }

    ui->label_caliTips->setText("1.使磁强记远离含铁、带电等磁性干扰源；\n2.开始校准。");

    return true;
}


extern void ACS_CalMagRefB(double adbMagRef[3],double adbMagB[3],double adbGyroW[3], double adbInterval);
extern void ACSATTD_MagMCalibrate(double adbXk[12], double adbPk[12 * 12], double adbMagMBb[3], double adbRefBb[3]);
int mag_cali(std::vector<double> &x,
             std::vector<double> &y,
             std::vector<double> &z,
             double *out,
             int out_size);

bool MainWindow::CaliRun(struct TyGyrMagData *data)
{
    int i = 0, j = 0, k = 0;
    QTime now = QTime::currentTime();

    switch (m_caliState) {

    case TY_CALI_STATE_OFF: /* 校准停用状态，啥都不干 */
        return true;
        break;

    case TY_CALI_STATE_CANCEL:  /* 取消校准过渡态 */
        CaliInit();
        ui->pushButton_cali->setEnabled(m_readerThread.isRunning());
        ui->pushButton_caliCancel->setEnabled(false);
        m_caliState = TY_CALI_STATE_OFF;
        break;

    case TY_CALI_STATE_INIT: /* 校准初始化状态 */
    {
        /* 初始化相关数据和控件 */
        CaliInit();

        ui->pushButton_cali->setEnabled(false);
        ui->pushButton_caliCancel->setEnabled(true);

        /* 采集频率固定为10Hz */
        ui->spinBox_AcqFreq->setValue(10);

        /* 记录校准开始时间（第一个报文的接收时间），便于后续陀螺校准的时间判断 */
        m_caliStartTime = now;

        /* 切换到陀螺仪校准状态 */
        m_caliState = TY_CALI_STATE_MAG;
        ui->label_caliTips->setText("磁强计校准中……\n请将磁强计沿各轴分别旋转360°以上。");

        break;
    }


    case TY_CALI_STATE_MAG: /* 磁强计校准 */
    {

        double magMin[3];   /**< 最小值，单位：mG */
        double magMax[3];   /**< 最大值，单位：mG */
        double magBias[3];  /**< 零偏，单位：mG */
        std::vector<double> mx, my, mz;


        /* 保存数据 */
        m_caliGyrMagData.push_back(*data);

        /* 等待数据量满足要求（各轴均旋转了360度以上，利用角速度积分计算角度） */
        for (i = 0; i < 3; i++) {
            m_caliGyrdegCur[i] += data->gyr[0].gyr[i] * 0.1;    /* 当前的角度 */
            m_caliGyrdegMin[i] = MIN(m_caliGyrdegCur[i], m_caliGyrdegMin[i]); /* 最小角度，为负的话表示反转过 */
            m_caliGyrdegMax[i] = MAX(m_caliGyrdegCur[i], m_caliGyrdegMax[i]); /* 最大角度 */

            m_caliGyrDegProgressBar[i]->setValue(MIN(360, m_caliGyrdegMax[i] - m_caliGyrdegMin[i]));
        }
        ui->progressBar_dataCnt->setValue(MIN(300, m_caliGyrMagData.size()));

        if (m_caliGyrDegProgressBar[0]->value() < 360
                || m_caliGyrDegProgressBar[1]->value() < 360
                || m_caliGyrDegProgressBar[2]->value() < 360
                || m_caliGyrMagData.size() < 300) {
            break;
        }

        for (i = 0; i < 2; i++) {

            mx.clear();
            my.clear();
            mz.clear();

            for (k = 0; k < m_caliGyrMagData.size(); k++) {
                mx.push_back(m_caliGyrMagData[k].mag[i].data[0]);
                my.push_back(m_caliGyrMagData[k].mag[i].data[1]);
                mz.push_back(m_caliGyrMagData[k].mag[i].data[2]);
            }

            mag_cali(mx, my, mz, m_caliMagCaliXk[i], 12);

            for (k = 0; k < 12; k++) {
                qDebug("m_caliMagCaliXk[%d][%d] = %.15lf", i, k, m_caliMagCaliXk[i][k]);
                m_caliMagParamSpinBox[i][k]->setValue(m_caliMagCaliXk[i][k]);
            }
        }


        /* 磁强计校准完成，进入下一阶段 */
        m_caliState = TY_CALI_STATE_DONE;
        ui->label_caliTips->setText("磁强计校准完成！");

        break;

    }

    case TY_CALI_STATE_DONE:
    {
        m_caliState = TY_CALI_STATE_OFF;
        ui->pushButton_cali->setEnabled(m_readerThread.isRunning());
        ui->pushButton_caliCancel->setEnabled(false);
        break;
    }

    default:
        break;
    }

    return true;
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
        ui->pushButton_cali->setEnabled(true);
        ui->pushButton_caliCancel->setEnabled(false);
        ui->pushButton_loadCaliData->setEnabled(true);
        ui->pushButton_saveCaliData->setEnabled(true);

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
    ui->pushButton_cali->setEnabled(false);
    ui->pushButton_loadCaliData->setEnabled(false);
    ui->pushButton_saveCaliData->setEnabled(false);


    showTips("关闭CAN适配器成功！");
}

void MainWindow::on_pushButton_ResetDev_clicked()
{
#if 0
    if (! m_tyCan.resetDev()) {
        showTips("复位CAN失败！");
        return;
    }

    showTips("复位CAN成功！");
#endif
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

void MainWindow::on_pushButton_cali_clicked()
{
    m_caliState = TY_CALI_STATE_INIT;
}

void MainWindow::on_pushButton_saveCaliData_clicked()
{
    int i = 0, j = 0;
    struct mmc5983_cali_data data[2];
    struct ty_can_frame req;

    memset(data, 0, sizeof(data));
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            data[i].m[0][j] = m_caliMagCaliXk[i][0 + j];
            data[i].m[1][j] = m_caliMagCaliXk[i][3 + j];
            data[i].m[2][j] = m_caliMagCaliXk[i][6 + j];
            data[i].v[j]    = m_caliMagCaliXk[i][9 + j] / 0.0625; /* / 0.0625 */
            qDebug("save data[%d].v[%d] = %d", i, j, data[i].v[j]);
        }
    }

    memset(&req, 0, sizeof(req));
    req.can_id.ext.src = 0;
    req.can_id.ext.dest = 0x06;
    req.can_id.ext.info = 0;
    req.can_id.ext.type = 0x01;
    req.data_type = 0x0F;
    req.data_id = 0x02;
    req.data_len = sizeof(data);
    memcpy(req.data, data, sizeof(data));

    m_tyCan.sendFrame(&req);
}


void MainWindow::on_pushButton_caliCancel_clicked()
{
    m_caliState = TY_CALI_STATE_CANCEL;
}
