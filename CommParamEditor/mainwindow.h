#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "CommParamDef.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
        void on_checkBox2G_stateChanged(int arg1);

        void on_checkBox3G_stateChanged(int arg1);

        void on_checkBox4G_stateChanged(int arg1);

        void on_checkBoxSameParam_stateChanged(int arg1);

        void on_lineEdit2GApn_textChanged(const QString &arg1);

        void on_lineEdit2GUsername_textChanged(const QString &arg1);

        void on_lineEdit2GPassword_textChanged(const QString &arg1);

        void on_lineEdit2GSeverIP1_textChanged(const QString &arg1);

        void on_lineEdit2GSeverPort1_textChanged(const QString &arg1);

        void on_lineEdit2GSeverIP2_textChanged(const QString &arg1);

        void on_lineEdit2GSeverIP3_textChanged(const QString &arg1);

        void on_lineEdit2GSeverPort3_textChanged(const QString &arg1);

        void on_lineEdit2GProxyIP_textChanged(const QString &arg1);

        void on_lineEdit2GProxyPort_textChanged(const QString &arg1);

        void on_lineEdit2GHeartInt_textChanged(const QString &arg1);

        void on_lineEdit2GRedialInt_textChanged(const QString &arg1);

        void on_lineEdit2GRedialCnt_textChanged(const QString &arg1);

        void on_lineEdit2GSeverPort2_textChanged(const QString &arg1);

        void on_pushButtonOpen_clicked();

        void on_pushButtonSave_clicked();

        void on_comboBoxNetOperator_currentIndexChanged(int index);

        void on_pushButtonSaveAs_clicked();

private:
    void sync2GParamTo34G();
    void set2GParamEnabled(bool enable);
    void set3GParamEnabled(bool enable);
    void set4GParamEnabled(bool enable);
    void loadFile(const QString &fpath);
    void saveFile(const QString &fpath);
    void syncFormToParam(int mnc);
    void syncParamToForm(int mnc);
    QString ipToString(struct in_addr &ip);
    void stringToIp(const QString &str, struct in_addr &ip);

private:
    Ui::MainWindow *ui;

    QString programName;
    QString filename;
    COMMPARAMCFG cfg;
    int lastMnc;
};

#endif // MAINWINDOW_H
