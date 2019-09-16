#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButtonSelect_clicked();
    void on_pushButtonFuck_clicked();

    void showTips(const QString &tips);
    bool decryptFile(const QString &filePath);
    bool isEncryptFile(const QString &filePath);
    bool IsDecryptSucess(const QString &oldFile, const QString &newFile);


private:
    Ui::MainWindow *ui;

    QString filePath;
};

#endif // MAINWINDOW_H
