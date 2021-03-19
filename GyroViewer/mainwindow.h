#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <stdint.h>
#include <QMainWindow>
#include <QVariant>
#include <string.h>

#include "tycan.h"
#include "tygyro.h"

#include "qcustomplot.h"


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define ABS(n)   ((n) > 0 ? (n) : (-1 * (n)))


#define DATA_MAX_NUM 200


struct TyGryData {
    bool valid;
    double gyr[3];
    double acc[3];
    double temp;
};

struct TyMagData {
    bool valid;
    double data[3];
    double temp;
};

struct TyGyrMagData {
    uint32_t time;
    struct TyGryData gyr[2];
    struct TyMagData mag[2];
};



namespace Ui {
    class MainWindow;
}


Q_DECLARE_METATYPE(ty_gyr_tele_data);



class DataRecvThread : public QThread
{
    Q_OBJECT

public:
    DataRecvThread(QObject *parent = Q_NULLPTR)
    {
        m_tyCan = NULL;
        m_isRunning = false;
        m_acqFreq = 4;
        m_acqEn = true;
    }

    ~DataRecvThread() {

    }

    void stop()
    {
        m_isRunning = false;
    }

    void setCan(TyCan *can)
    {
        m_tyCan = can;
    }

    void setAcqFreq(uint8_t freq)
    {
        if (freq < 1) {
            m_acqFreq = 1;
        } else if (freq > 100) {
            m_acqFreq = 100;
        } else {
            m_acqFreq = freq;
        }
    }

    void setAcqEn(bool en)
    {
        m_acqEn = en;
    }

    virtual void run()
    {
        QVariant var;
        struct ty_can_frame rsp;
        struct ty_can_frame req;
        struct ty_gyr_tele_data data;
        qint64 now = 0;
        qint64 time = 0;

        if (! m_tyCan) {
            return;
        }

        memset(&req, 0, sizeof(req));
        req.can_id.ext.src = 0;
        req.can_id.ext.dest = 0x06;
        req.can_id.ext.info = 0;
        req.can_id.ext.type = 0x01;
        req.data_type = 0x0;
        req.data_id = 0x01;
        req.data_len = 6;

        m_isRunning = true;
        while (m_isRunning) {

            if (m_acqEn) {
                now = QDateTime::currentMSecsSinceEpoch();
                m_tyCan->sendFrame(&req);
            }

            memset(&rsp, 0, sizeof(rsp));
            if (m_tyCan->recvFrame(&rsp, 1000)) {

                if (0x06 == rsp.can_id.ext.src
                        && 0x35 == rsp.data_type && 1 == rsp.data_id
                        && sizeof(data) == rsp.data_len)
                {

                    memcpy(&data, rsp.data, sizeof(data));
                    var.setValue(data);
                    emit sendRawTeleData(var);
                }
            }

            if (m_acqEn) {
                time = 1000 / m_acqFreq - (QDateTime::currentMSecsSinceEpoch() - now);
                if (time > 0) { // 小于等于0表示所设采集频率过高
                    msleep(time);
                }
            }
        }
    }


signals:
    void sendRawTeleData(QVariant var);

private:
    bool m_isRunning;
    bool m_acqEn;
    TyCan *m_tyCan;
    uint8_t m_acqFreq;

};


class AxisData
{
public:
    AxisData(int size = DATA_MAX_NUM)
    {
        m_max_size = size;

        m_key_max = 0.0;
        m_key_min = 0.0;
        m_val_max = 0.0;
        m_val_min = 0.0;
    }

    ~AxisData() {
        m_key.clear();
        m_val.clear();

        m_key_max = 0.0;
        m_key_min = 0.0;
        m_val_max = 0.0;
        m_val_min = 0.0;
    }

    void clear(void)
    {
        m_key.clear();
        m_val.clear();

        m_key_max = 0;
        m_key_min = 0;
        m_val_max = 0;
        m_val_min = 0;
    }

    void addData(double key, double val)
    {
        int i = 0;

        if (m_key.size() >= m_max_size) {
            m_key.pop_front();
            m_val.pop_front();
        }

        m_key.push_back(key);
        m_val.push_back(val);

        /* 重新计算最大值和最小值 */
        m_key_max = m_key[0];
        m_key_min = m_key[0];
        m_val_max = m_val[0];
        m_val_min = m_val[0];

        for (i = 0; i < m_key.size(); i++) {
            m_key_max = MAX(m_key_max, m_key[i]);
            m_key_min = MIN(m_key_min, m_key[i]);
            m_val_max = MAX(m_val_max, m_val[i]);
            m_val_min = MIN(m_val_min, m_val[i]);
        }
    }

public:
    int m_max_size;         /**< 数组最大大小 */
    QVector<double> m_key;  /**< KEY数组 */
    QVector<double> m_val;  /**< VALUE数组 */
    double m_key_max;       /**< KEY的最大值 */
    double m_key_min;       /**< KEY的最小值 */
    double m_val_max;       /**< VALUE的最大值 */
    double m_val_min;       /**< VALUE的最小值 */

};


enum TyCaliStateEnum {
    TY_CALI_STATE_OFF = 0,  /**< 未进行校准 */
    TY_CALI_STATE_INIT,     /**< 校准初始状态 */
    TY_CALI_STATE_GYR,      /**< 陀螺仪校准阶段 */
    TY_CALI_STATE_MAG,      /**< 磁强计校准阶段 */
    TY_CALI_STATE_DONE,     /**< 校准完成状态 */
    TY_CALI_STATE_CANCEL,   /**< 取消校准过渡态 */
};

struct MagCaliData {
    double m[3][3]; /**< 零偏矩阵 */
    double v[3];    /**< 零偏向量 */
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showTips(const QString &tips);

public slots:
    void recvTeleData(QVariant var);


private slots:
    void on_pushButton_OpenDev_clicked();
    void on_pushButton_CloseDev_clicked();
    void on_pushButton_ResetDev_clicked();
    void on_spinBox_AcqFreq_valueChanged(int arg1);
    void on_comboBox_WorkMode_currentIndexChanged(int index);
    void on_pushButton_cali_clicked();
    void on_pushButton_saveCaliData_clicked();
    void on_pushButton_caliCancel_clicked();

private:
    void clearData(void);
    bool ConvertTeleData(const struct ty_gyr_tele_data *tele, struct TyGyrMagData *data);
    bool CaliInit(void);
    bool CaliRun(struct TyGyrMagData *data);



private:
    Ui::MainWindow *ui;

    TyCan m_tyCan;                  /**< CAN收发器 */

    DataRecvThread m_readerThread;  /**< 数据读取线程 */

    FILE *m_gyrFile[2];             /**< 陀螺仪数据保存文件 */
    FILE *m_magFile[2];             /**< 磁强计数据保存文件 */
    FILE *m_accFile[2];             /**< 加速度计数据保存文件 */


    /***************************************************************************
     * 图标绘画相关
     ***************************************************************************/
    AxisData m_magData[2][3];       /**< 磁强计坐绘画数据 */
    AxisData m_gyrData[2][3];       /**< 陀螺仪坐绘画数据 */
    AxisData m_accData[2][3];       /**< 加速度计绘画数据 */

    QCustomPlot *m_gyrPlot[2][3];   /**< 陀螺仪的曲线图像 */
    QCustomPlot *m_accPlot[2][3];   /**< 加速度计的曲线图像 */
    QCustomPlot *m_magPlot[2][3];   /**< 磁强计的曲线图像 */


    /**************************************************************************
     * 校准相关
     **************************************************************************/
    enum TyCaliStateEnum m_caliState;               /**< 校准状态 */

    QTime m_caliStartTime;                          /**< 校准开始时间 */

    QVector<struct TyGyrMagData> m_caliGyrMagData;  /**< 磁强计/陀螺仪校准所需的数据 */

    double m_caliGyrBias[2][3];                     /**< 陀螺仪的零偏，单位：角度/秒 */
    QLabel *m_caliGyrBiasLabel[2][3];               /**< 陀螺仪的零偏显示控件 */

    double m_caliGyrdegCur[3];                      /**< 陀螺仪的当前旋转角度 */
    double m_caliGyrdegMin[3];                      /**< 陀螺仪的旋转角度最小值 */
    double m_caliGyrdegMax[3];                      /**< 陀螺仪的旋转角度最大值 */
    QProgressBar *m_caliGyrDegProgressBar[3];       /**< 陀螺仪的旋转角度显示控件 */

    double m_caliMagCaliXk[2][12];                  /**< 磁强计的校准参数 */
    double m_caliMagCaliPk[2][144];                 /**< 磁强计的校准中间变量 */
    QDoubleSpinBox *m_caliMagParamSpinBox[2][12];   /**< 磁强计的偏差矩阵显示控件 */
};



#endif // MAINWINDOW_H
