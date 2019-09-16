#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "decryptor.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isSysSupportDecrypt();


private slots:
    void on_pushButtonSelect_clicked();
    void on_pushButtonSelectDir_clicked();
    void on_pushButtonFuck_clicked();

private:
    void showTips(const QString &tips);

    bool decryptFile(const QString &filePath);
    bool decryptFiles(const QStringList &fileList);
    bool decryptDir(const QString &dirPath, const QStringList &filters);


    QStringList getSupportFileTypes();

private:
    Ui::MainWindow *ui;

    QVector<Decryptor *> decryptors;    /**< 解密器数组 */
    QStringList selectFiles;            /**< 待解密的文件列表 */
    QString selectDir;

    unsigned int totalCnt;
    unsigned int SuccessCnt;
    unsigned int FailedCnt;
};

#endif // MAINWINDOW_H
