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


#define DATA_MAX_NUM 200


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



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showTips(const QString &tips);

public slots:
    void recvRawTeleData(QVariant var);


private slots:
    void on_pushButton_OpenDev_clicked();
    void on_pushButton_CloseDev_clicked();
    void on_pushButton_ResetDev_clicked();
    void on_spinBox_AcqFreq_valueChanged(int arg1);
    void on_comboBox_WorkMode_currentIndexChanged(int index);

private:
    void clearData(void);


private:
    Ui::MainWindow *ui;

    TyCan m_tyCan;

    DataRecvThread m_readerThread;

    FILE *m_gyrFile[2];
    FILE *m_magFile[2];
    FILE *m_accFile[2];

    AxisData m_magData[2][3];
    AxisData m_gyrData[2][3];
    AxisData m_accData[2][3];

    QCustomPlot *m_gyrPlot[2][3];
    QCustomPlot *m_accPlot[2][3];
    QCustomPlot *m_magPlot[2][3];
};

#endif // MAINWINDOW_H
