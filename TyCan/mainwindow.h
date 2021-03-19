#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <stdint.h>
#include <QMainWindow>
#include <QVariant>
#include <QThread>
#include <string.h>

#include "tycan.h"
#include "tygyro.h"


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define ABS(n)   ((n) > 0 ? (n) : (-1 * (n)))


#define DATA_MAX_NUM 200


struct CanMsg {
    int num;
    CAN_OBJ objs[50];
};


namespace Ui {
    class MainWindow;
}


Q_DECLARE_METATYPE(CanMsg);



class DataRecvThread : public QThread
{
    Q_OBJECT

public:
    DataRecvThread(QObject *parent = Q_NULLPTR)
    {
        m_tyCan = NULL;
        m_isRunning = false;
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

    virtual void run()
    {
        QVariant var;
        struct CanMsg msg;

        if (! m_tyCan) {
            return;
        }

        m_isRunning = true;
        while (m_isRunning) {

            memset(msg.objs, 0, sizeof(msg.objs));
            msg.num = m_tyCan->recvRawFrame(msg.objs, sizeof(msg.objs) / sizeof(msg.objs[0]), 500);

            qDebug("recv num: %d", msg.num);

            if (msg.num > 0) {
                var.setValue(msg);
                emit sendRawTeleData(var);
            }
        }
    }


signals:
    void sendRawTeleData(QVariant var);

private:
    bool m_isRunning;
    TyCan *m_tyCan;
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
    void on_pushButton_Clear_clicked();
    void on_pushButton_Send_clicked();
    void on_comboBox_FltrMode_currentIndexChanged(int index);

private:
    QByteArray HexStringToByteArray(QString HexString);



private:
    Ui::MainWindow *ui;

    TyCan m_tyCan;                  /**< CAN收发器 */
    DataRecvThread m_readerThread;  /**< 数据读取线程 */
};



#endif // MAINWINDOW_H
