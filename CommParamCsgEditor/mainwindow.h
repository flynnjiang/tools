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

private:
    void syncFormToParam(int mnc);
    void syncParamToForm(int mnc);
    QString ipToString(struct in_addr &ip);
    void stringToIp(const QString &str, struct in_addr &ip);

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonSaveAs_clicked();

    void on_comboBoxNetOperator_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QString programName;
    QString filename;
    COMMPARAMCFG_CSG18 cfg;
    int lastMnc;
};

#endif // MAINWINDOW_H
